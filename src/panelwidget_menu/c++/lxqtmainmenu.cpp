/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2011 Razor team
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#include "lxqtmainmenu.h"
#include "lxqtmainmenuconfiguration.h"
#include "xdgcachedmenu.h"

#include "core/ICore.h"

#include <QDebug>

#include <lxqt/lxqtpowermanager.h>
#include <lxqt/lxqtscreensaver.h>
//TODO: uncoment wen lxqt-globalkeys became available for QT5
//#include <lxqt-globalkeys.h>
#include <lxqt/lxqtxfitman.h>

#include <qt5xdg/xdgicon.h>
#include <qt5xdg/xdgdesktopfile.h>
#include <qt5xdg/xdgmenuwidget.h>
#include <qt5xdg/xdgdesktopfile.h>
#include <qt5xdg/xmlhelper.h>

#include <usModuleContext.h>
#include <usGetModuleContext.h>
#include <usServiceReference.h>
#include <usServiceProperties.h>


#include <QSettings>
#include <QFileInfo>
#include <QAction>
#include <QTimer>
#include <QMessageBox>
#include <QPixmap>
#include <QStack>
#include <QCursor>

#define DEFAULT_SHORTCUT "Alt+F1"

using namespace us;

/************************************************

 ************************************************/
LxQtMainMenu::LxQtMainMenu() :
mMenu(0),
//TODO: uncoment wen lxqt-globalkeys became available for QT5
//    mShortcut(0),
mLockCascadeChanges(false) {
    // Services find
    ModuleContext * context = GetModuleContext();
    { // Find Panel
        ServiceReference<IPanel> ref =
                context->GetServiceReference<IPanel>();
        if (!ref) {
            qWarning() << "Unable to find the IPanel service.";
        } else {
            m_panel = qobject_cast<QWidget>(context->GetService(ref));
        }
    }
    { // Find settings
        ServiceReference<Core::ISettingsProfile> ref =
                context->GetServiceReference<Core::ISettingsProfile>();
        if (!ref) {
            qWarning() << "Unable to find the SettingsProfile service.";
        } else {
            Core::ISettingsProfile * settingsProfile = context->GetService(ref);
            m_settings = settingsProfile->getSettingsOf("Panel/ClasicMenu");
        }
    }

    mMenuCache = NULL;
    mMenuCacheNotify = 0;


    mButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    connect(&mButton, SIGNAL(clicked()), this, SLOT(showMenu()));

    mPowerManager = new LxQt::PowerManager(this);
    mScreenSaver = new LxQt::ScreenSaver(this);

    settingsChanged();

    //TODO: uncoment wen lxqt-globalkeys became available for QT5
    //    connect(mShortcut, SIGNAL(activated()), this, SLOT(showHideMenu()));
    //    connect(mShortcut, SIGNAL(shortcutChanged(QString,QString)), this, SLOT(shortcutChanged(QString,QString)));
}

/************************************************

 ************************************************/
LxQtMainMenu::~LxQtMainMenu() {
#ifdef HAVE_MENU_CACHE
    if (mMenuCache) {
        menu_cache_remove_reload_notify(mMenuCache, mMenuCacheNotify);
        menu_cache_unref(mMenuCache);
    }
#endif
}

/************************************************

 ************************************************/
void LxQtMainMenu::showHideMenu() {
    if (mMenu && mMenu->isVisible())
        mMenu->hide();
    else
        showMenu();
}

/************************************************

 ************************************************/
void LxQtMainMenu::shortcutChanged(const QString &/*oldShortcut*/, const QString &newShortcut) {
    if (!newShortcut.isEmpty()) {
        mLockCascadeChanges = true;

        settings()->setValue("dialog/shortcut", newShortcut);
        settings()->sync();

        mLockCascadeChanges = false;
    }
}

/************************************************

 ************************************************/
void LxQtMainMenu::showMenu() {
    if (!mMenu)
        return;

    int x = 0, y = 0;


    x = mButton.mapToGlobal(QPoint(0, 0)).x();
    y = panel()->globalGometry().top() - mMenu->sizeHint().height();


    // Just using Qt`s activateWindow() won't work on some WMs like Kwin.
    // There are two solutions:
    //  activate window with Qt call and then execute menu 1ms later using timer,
    //  or use native X11 API calls:
    //xfitMan().raiseWindow(mButton.effectiveWinId());
    mButton.activateWindow();
    mMenu->exec(QPoint(x, y));
}

#ifdef HAVE_MENU_CACHE
// static

void LxQtMainMenu::menuCacheReloadNotify(MenuCache* cache, gpointer user_data) {
    reinterpret_cast<LxQtMainMenu*> (user_data)->buildMenu();
}
#endif

/************************************************

 ************************************************/
void LxQtMainMenu::settingsChanged() {
    if (mLockCascadeChanges)
        return;

    if (settings()->value("showText", false).toBool()) {
        mButton.setText(settings()->value("text", "Start").toString());
        mButton.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        mButton.setText("");
        mButton.setToolButtonStyle(Qt::ToolButtonIconOnly);
    }

    mLogDir = settings()->value("log_dir", "").toString();

    QString mMenuFile = settings()->value("menu_file", "").toString();
    if (mMenuFile.isEmpty())
        mMenuFile = XdgMenu::getMenuFileName();

#ifdef HAVE_MENU_CACHE
    menu_cache_init(0);
    if (mMenuCache) {
        menu_cache_remove_reload_notify(mMenuCache, mMenuCacheNotify);
        menu_cache_unref(mMenuCache);
    }
    mMenuCache = menu_cache_lookup(mMenuFile.toLocal8Bit());
    mMenuCacheNotify = menu_cache_add_reload_notify(mMenuCache, (MenuCacheReloadNotify) menuCacheReloadNotify, this);
#else
    mXdgMenu.setEnvironments(QStringList() << "X-LXQT" << "LxQt");
    mXdgMenu.setLogDir(mLogDir);

    bool res = mXdgMenu.read(mMenuFile);
    connect(&mXdgMenu, SIGNAL(changed()), this, SLOT(buildMenu()));
    if (res) {
        QTimer::singleShot(1000, this, SLOT(buildMenu()));
    } else {
        QMessageBox::warning(0, "Parse error", mXdgMenu.errorString());
        return;
    }
#endif

    QString shortcut = settings()->value("shortcut", DEFAULT_SHORTCUT).toString();
    if (shortcut.isEmpty())
        shortcut = DEFAULT_SHORTCUT;

    //TODO: uncoment wen lxqt-globalkeys became available for QT5
    //    if (!mShortcut)
    //        mShortcut = GlobalKeyShortcut::Client::instance()->addAction(shortcut, QString("/panel/%1/show_hide").arg(settings()->group()), tr("Show/hide main menu"), this);
    //    else if (mShortcut->shortcut() != shortcut)
    //    {
    //        mShortcut->changeShortcut(shortcut);
    //    }

    realign();
}

/************************************************

 ************************************************/
void LxQtMainMenu::buildMenu() {
    qDebug() << "BUILD_MENU";
#ifdef HAVE_MENU_CACHE
    XdgCachedMenu* menu = new XdgCachedMenu(mMenuCache, &mButton);
#else
    XdgMenuWidget *menu = new XdgMenuWidget(mXdgMenu, "", &mButton);
#endif
    menu->setObjectName("TopLevelMainMenu");
    menu->setStyle(&mTopMenuStyle);

    menu->addSeparator();

    QMenu* leaveMenu = menu->addMenu(XdgIcon::fromTheme("system-shutdown"), tr("Leave"));
    leaveMenu->addActions(mPowerManager->availableActions());
    menu->addActions(mScreenSaver->availableActions());

    QMenu *oldMenu = mMenu;
    mMenu = menu;
    delete oldMenu;
}

/************************************************

 ************************************************/
QDialog *LxQtMainMenu::configureDialog() {
    return new LxQtMainMenuConfiguration(*settings(), DEFAULT_SHORTCUT);
}


#undef DEFAULT_SHORTCUT
