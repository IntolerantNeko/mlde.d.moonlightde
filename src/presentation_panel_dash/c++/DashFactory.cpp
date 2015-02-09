/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * 
 * Authors:
 *  Alexis López Zubieta
 * 
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

#include "DashFactory.h"

#include <QDebug>

DashFactory::DashFactory() : m_dash(NULL) {
}

DashFactory::~DashFactory() {
    if (m_dash != NULL) {
        delete(m_dash);
    }
}

QWidget* DashFactory::getLauncher(const QString &menuFileName) {
    //  Menu file name it's not important when using XdgDesktopFileCach
    if (m_dash == NULL) {
        m_dash = new Dash();
    }
    return m_dash;
}