
add_library(dock-showdesktop SHARED
    showdesktop.cpp
    showdesktop.h
)

target_link_libraries(dock-showdesktop PRIVATE
    dde-shell-frame
)

ds_install_package(PACKAGE org.deepin.ds.dock.showdesktop TARGET dock-showdesktop)
