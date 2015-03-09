/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
 * Authors:
 *      Alexis López Zubieta
 * This file is part of Moonlight Desktop Environment.
 *
 * Moonlight Desktop Environment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight Desktop Environment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight Desktop Environment. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PANELIMPL_H
#define PANELIMPL_H

#include "WidgetsTracker.h"

#include "presentation_panel/Panel.h"
#include "presentation_panel/WidgetFactory.h"

#include <usModuleContext.h>
#include <usServiceTracker.h>
#include <usGetModuleContext.h>

#include <QMap>
#include <QRect>
#include <QWidget>
#include <QPointer>
#include <QSettings>

/**
 * A very static implementation of a desktop panel.
 */
class PanelImpl : public QWidget, public presentation_panel::Panel {
    Q_OBJECT

public:
    explicit PanelImpl(QWidget *parent = 0);

    ~PanelImpl();

    virtual int desktop();
    virtual void setDesktop(int desktop);


    void updateWidgetsOrder();
    void writeWidgetsOrder();
    bool configExist(QString key);
    virtual void addWidgetFactory(presentation_panel::WidgetFactory* widgetFactory);
    virtual void removeWidgetFactory(presentation_panel::WidgetFactory* widgetFactory);

    virtual QRect geometry();
    virtual void setGeometry(QRect geometry);

public slots:
    void setupWindowFlags();
    void adjustSizeToScreen();
    void requestExclusiveScreenArea();


protected:
    bool event(QEvent *event);
    void updateLayout();

private:
    QMap<QString, presentation_panel::WidgetFactory *> m_Factories;
    QMap<QString, QPointer<QWidget> > m_Widgets;

    
    /* Panel properties */
    QRect m_Geometry;
    int m_Desktop;
    QSettings *moduleSettings; 
    
    WidgetsTracker m_WidgetTracker;
};

#endif // PANELIMPL_H
