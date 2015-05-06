/*
 * Copyright (C) 2015 Moonlight Desktop Environment Team
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

#ifndef GVFSVOLUMEMANAGER_H
#define	GVFSVOLUMEMANAGER_H

#include "GVFSMount.h"
#include "GVFSVolume.h"

#include <gio/gio.h>

#include <QHash>
#include <QString>
#include <QObject>

class GVFSVolumeManager : public QObject {
    Q_OBJECT
public:
    GVFSVolumeManager();
    virtual ~GVFSVolumeManager();

    QList<GVFSVolume *> volumes();
    QList<GVFSMount *> mounts();

Q_SIGNALS:
    void mountAdded(QString name);
    void mountRemoved(QString name);
    //void mount_changed(QString name);
    void mountPremount(QString name);

    void volumeAdded(QString name);
    void volumeRemoved(QString name);

protected:
    void handle_mount_added(GMount *gmount);
    static void on_g_mount_added(GVolumeMonitor *volume_monitor, GMount *mount, GVFSVolumeManager *volumeManager);
    void handle_mount_removed(GMount *gmount);
    static void on_g_mount_removed(GVolumeMonitor *volume_monitor, GMount *mount, GVFSVolumeManager *volumeManager);
    //    void handle_mount_changed(GMount *gmount);
    //    static void on_mount_changed(GVolumeMonitor *volume_monitor, GMount *mount, GVFSVolumeManager *volumeManager);
    void handle_mount_preunmount(GMount *gmount);
    static void on_g_mount_premount(GVolumeMonitor *volume_monitor, GMount *mount, GVFSVolumeManager *volumeManager);

    void handle_volume_added(GVolume *volume);
    static void on_g_volume_added(GVolumeMonitor *volume_monitor, GVolume *volume, GVFSVolumeManager *volumeManager);
    void handle_volume_removed(GVolume *volume);
    static void on_g_volume_removed(GVolumeMonitor *volume_monitor, GVolume *volume, GVFSVolumeManager *volumeManager);

    //    void handle_volume_changed(GVolume *volume);
    //    static void on_g_volume_changed(GVolumeMonitor *volume_monitor, GVolume *volume, GVFSVolumeManager *volumeManager);

    // TODO: Check if it's necesary to handle the signals bellow
    /*
    static void on_drive_connected(GVolumeMonitor *volume_monitor, GDrive *drive);
    static void on_drive_disconnected(GVolumeMonitor *volume_monitor, GDrive *drive);
    static void on_drive_changed(GVolumeMonitor *volume_monitor, GDrive *drive);
    static void on_drive_eject_button(GVolumeMonitor *volume_monitor, GDrive *drive);*/

private:
    GVolumeMonitor *m_VolumeMonitor;
//    QHash<QString, GVolume * > m_Volumes;
//    QHash<QString, GMount * > m_Mounts;

};

#endif	/* GVFSVOLUMEMANAGER_H */

