
add_library(dock-launcheritem SHARED
    launcheritem.cpp
    launcheritem.h
)

target_link_libraries(dock-launcheritem PRIVATE
    dde-shell-frame
)

ds_install_package(PACKAGE org.deepin.ds.dock.launcheritem TARGET dock-launcheritem)
