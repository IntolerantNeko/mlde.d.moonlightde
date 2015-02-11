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

#ifndef WIDGETSTRACKER_H
#define	WIDGETSTRACKER_H


#include "presentation_panel/WidgetFactory.h"

#include <usServiceTracker.h>
#include <usServiceTrackerCustomizer.h>

#include <QPointer>

class PanelImpl;
class WidgetsTracker : public us::ServiceTrackerCustomizer<presentation_panel::WidgetFactory> {
public:
    WidgetsTracker(PanelImpl *panel);
    virtual ~WidgetsTracker();

    virtual presentation_panel::WidgetFactory* AddingService(const us::ServiceReference<presentation_panel::WidgetFactory>& reference);

    virtual void ModifiedService(const us::ServiceReference<presentation_panel::WidgetFactory>& reference, presentation_panel::WidgetFactory *service);

    virtual void RemovedService(const us::ServiceReference<presentation_panel::WidgetFactory>& reference, presentation_panel::WidgetFactory *service);

private:
    QPointer<PanelImpl> m_Panel;
    us::ModuleContext* m_Context;
    us::ServiceTracker<presentation_panel::WidgetFactory> * m_tracker;
};

#endif	/* WIDGETSTRACKER_H */

