#pragma once

#include "dsglobal.h"
#include "abstractwindowmonitor.h"
#include <qobject.h>

DS_BEGIN_NAMESPACE
namespace dock {
class WaylandWindowMonitor : public AbstractWindowMonitor
{
public:
    explicit WaylandWindowMonitor(QObject* parent = nullptr) : AbstractWindowMonitor(parent) {};

};
}
DS_END_NAMESPACE
