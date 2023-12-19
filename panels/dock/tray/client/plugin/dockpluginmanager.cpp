// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dockpluginmanager_p.h"
#include "dockpluginsurface_p.h"

namespace dock {
DockPluginManager::DockPluginManager(::wl_registry *registry, uint32_t id, uint32_t version)
    : QtWayland::dock_plugin_manager_v1(registry, id, version)
    , QObject()
{
    init(registry, id, version);
}

DockPluginManager::~DockPluginManager()
{
}

void DockPluginManager::dock_plugin_manager_v1_position_changed(uint32_t dockPosition)
{
    if (dockPosition != m_dockPosition) {
        m_dockPosition = dockPosition;
        Q_EMIT dockPositionChnaged(m_dockPosition);
    }
}

void DockPluginManager::dock_plugin_manager_v1_display_mode_changed(uint32_t dockDisplayMode)
{
    if (dockDisplayMode != m_dockDisplayMode) {
        m_dockDisplayMode = dockDisplayMode;
        Q_EMIT dockDisplayModeChnaged(m_dockDisplayMode);
    }
}

void DockPluginManager::dock_plugin_manager_v1_hide_state_changed(uint32_t dockHideState)
{
    if (dockHideState != m_dockHideState) {
        m_dockHideState = dockHideState;
        Q_EMIT dockHideStateChanged(m_dockHideState);
    }
}


QtWaylandClient::QWaylandShellSurface* DockPluginManager::createPluginSurface(QtWaylandClient::QWaylandWindow *window)
{
    return new DockPluginSurface(this, window);
}
}
