/*
 * Copyright (C) 2014 Moonlight Desktop Environment Team
 * Authors:
 * Alexis López Zubieta
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


#include "Controller.h"
#include "SettingsProfile.h"
#include "ModuleManager.h"
#include "Environment.h"

#include <usGetModuleContext.h>
#include <usModuleContext.h>
#include <usServiceProperties.h>

#include <QDebug>
#include <QApplication>

using namespace us;

Controller::Controller(QString profile, QString aditionalLibsPath) {
    ModuleContext * context = GetModuleContext();

    // Core services creation
    settingsProfile = new SettingsProfile(profile);
    {
        ServiceProperties props;
        context->RegisterService<Core::ISettingsProfile>(settingsProfile, props);
    }
    moduleManager = new ModuleManager(aditionalLibsPath);
    {
        ServiceProperties props;
        context->RegisterService<Core::IModuleManager>(moduleManager, props);
    }

    environment = new Environment();
    {
        ServiceProperties props;
        context->RegisterService<Core::IEnvironment>(environment, props);
    }

    {
        ServiceProperties props;
        context->RegisterService<Core::IController>(this, props);
    }
}

void Controller::start() {
    // Loading modules
    QSettings * profile = settingsProfile->getSettingsOf();
    moduleManager->loadFromProfile(profile);

    emit(started());
}

void Controller::finish() {
    qApp->exit();
    emit(finishing());
}

Controller::~Controller() {
}

