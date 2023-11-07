#pragma once

#include "appitem.h"
#include "dsglobal.h"

#include <QObject>

DS_BEGIN_NAMESPACE
namespace dock {
class AppMenu : QObject
{
    Q_OBJECT
    AppMenu(QObject *parent = nullptr);
};
}

DS_END_NAMESPACE
