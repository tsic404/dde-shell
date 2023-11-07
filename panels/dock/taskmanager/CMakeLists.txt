# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(Qt${QT_VERSION_MAJOR} ${REQUIRED_QT_VERSION} COMPONENTS DBus)

file(
    GLOB taskmanager_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/dbus/am/org.desktopspec.ApplicationManager1.Application.xml
    PROPERTIES  INCLUDE dbus/types/am.h
                CLASSNAME Application
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/dbus/am/org.desktopspec.ApplicationManager1.xml
    PROPERTIES  INCLUDE dbus/types/am.h
                CLASSNAME ApplicationManager
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/dbus/am/org.desktopspec.ProcessGuesser1.xml
    PROPERTIES  INCLUDE dbus/types/am.h
                CLASSNAME ProcessGuesser
)

qt_add_dbus_interfaces(
    DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/dbus/am/org.desktopspec.ApplicationManager1.Application.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/dbus/am/org.desktopspec.ApplicationManager1.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/dbus/am/org.desktopspec.ProcessGuesser1.xml
)

add_library(dock-taskmanager SHARED ${taskmanager_sources} ${DBUS_INTERFACES})
target_link_libraries(dock-taskmanager PRIVATE
    dde-shell-frame
)

ds_install_package(PACKAGE org.deepin.ds.dock.taskmanager TARGET dock-taskmanager)
