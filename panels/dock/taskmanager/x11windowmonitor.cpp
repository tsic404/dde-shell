#include "x11windowmonitor.h"
#include "dsglobal.h"
#include <qobject.h>

DS_BEGIN_NAMESPACE
namespace dock {
X11WindowMonitor::X11WindowMonitor(QObject* parent)
    : AbstractWindowMonitor(parent)
{

}

void X11WindowMonitor::listenXEventUseXlib()
{

}
}

DS_END_NAMESPACE
