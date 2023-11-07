
#include "abstractwindow.h"
#include "abstractwindowmonitor.h"
#include "dsglobal.h"
#include "taskmanager.h"
#include "waylandwindowmonitor.h"
#include "x11windowmonitor.h"

#include <QGuiApplication>
#include <sys/types.h>

DS_BEGIN_NAMESPACE
namespace dock {

TaskManager::TaskManager(QObject* parent)
    : DContainment(parent)
{
    auto platformName = QGuiApplication::platformName();
    if ("wayland" == platformName) {
        m_windowMonitor.reset(new WaylandWindowMonitor(this));
    } else if ("xcb" == platformName) {
        m_windowMonitor.reset(new X11WindowMonitor(this));
    }
    connect(m_windowMonitor.data(), &AbstractWindowMonitor::windowAdded, this, &TaskManager::handleWindowAdded);
    connect(m_windowMonitor.data(), &AbstractWindowMonitor::windowChanged, this, &TaskManager::handleWindowChanged);
    connect(m_windowMonitor.data(), &AbstractWindowMonitor::windowRemoved, this, &TaskManager::handleWindowRemoved);
}

void TaskManager::handleWindowAdded(AbstractWindow* window)
{
    pid_t pid = window->pid();
}

}
DS_END_NAMESPACE
