#pragma once

#include "appitem.h"
#include "dsglobal.h"
#include "containment.h"
#include "abstractwindow.h"
#include "abstractwindowmonitor.h"

#include <QMap>

DS_BEGIN_NAMESPACE
namespace dock {

class TaskManager : public DContainment
{
    Q_OBJECT
public:
    explicit TaskManager(QObject* parent = nullptr);
    void requestDockAction(AppItem *item, bool docked);

    QStringList getDockedItems();
    virtual void init() override;
    virtual void load() override;

private Q_SLOTS:
    void handleWindowAdded(AbstractWindow* window);
    void handleWindowChanged(AbstractWindow* window);
    void handleWindowRemoved(AbstractWindow* window);

Q_SIGNALS:


private:
    bool m_windowSplit;                 // is window splited display, evey window own unique item
    AppItem* m_currentActiveItem;       // currnet activate item
    AppItems m_items;                   // all items and appid
    QScopedPointer<AbstractWindowMonitor> m_windowMonitor;
};

}
DS_END_NAMESPACE
