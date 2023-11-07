#pragma once

#include "dsglobal.h"
#include "abstractwindowmonitor.h"

DS_BEGIN_NAMESPACE
namespace dock {

class X11WindowMonitor : public AbstractWindowMonitor
{
public:
    explicit X11WindowMonitor(QObject* parent = nullptr);

private:
    void listenXEventUseXlib();

};
}
DS_END_NAMESPACE
