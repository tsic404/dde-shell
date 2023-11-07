#pragma once

#include "abstractwindow.h"
#include "dsglobal.h"

#include <qobject.h>
#include <qtmetamacros.h>

DS_BEGIN_NAMESPACE

namespace dock {
class AbstractWindowMonitor : public QObject
{
    Q_OBJECT
public:
    AbstractWindowMonitor(QObject* parent = nullptr);
Q_SIGNALS:
    void windowAdded(AbstractWindow* window);
    void windowChanged(AbstractWindow* window);
    void windowRemoved(AbstractWindow* window);
};
}
DS_END_NAMESPACE
