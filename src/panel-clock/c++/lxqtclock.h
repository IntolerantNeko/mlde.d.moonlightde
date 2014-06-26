/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2010-2013 Razor team
 * Authors:
 *   Christopher "VdoP" Regali
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *   Maciej Płaza <plaza.maciej@gmail.com>
 *   Kuzma Shapran <kuzma.shapran@gmail.com>
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

#ifndef LXQTCLOCK_H
#define LXQTCLOCK_H




#include "panel/IPanel.h"

#include <LXQt/lxqtrotatedwidget.h>

#include <QWidget>
#include <QString>
#include <QPointer>
#include <QSettings>

class QLabel;
class QDialog;
class QTimer;

class LxQtClockConfiguration;

class LxQtClock : public QWidget, public IClock {
    Q_OBJECT
public:
    LxQtClock();
    ~LxQtClock();

    virtual void mousePressEvent(QMouseEvent * event);

public slots:
    void updateTime();
    void settingsChanged();

    void showCalendar();
    void showSettings();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QPointer<QWidget> m_panel;
    QTimer* mClockTimer;
    QWidget *mContent;
    LxQt::RotatedWidget* mRotatedWidget;
    QLabel* mTimeLabel;
    QLabel* mDateLabel;
    QString mClockFormat;
    QString mToolTipFormat;
    QPointer<QWidget> mCalendarWidget;
    QString mTimeFormat;
    QString mDateFormat;
    bool mDateOnNewLine;
    bool mUseUTC;
    Qt::DayOfWeek mFirstDayOfWeek;

    QDateTime currentDateTime();
    void showTime(const QDateTime &);
    void restartTimer(const QDateTime&);

    QSettings *m_settings;
private slots:
    void updateMinWidth();
};

#endif
