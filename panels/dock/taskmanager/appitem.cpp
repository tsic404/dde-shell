#include "appitem.h"
#include "abstractwindow.h"
#include "dsglobal.h"
#include <qobject.h>

DS_BEGIN_NAMESPACE
namespace dock {
AppItem::AppItem(QString id, QObject *parent)
    : QObject(parent)
{

}

QString AppItem::icon()
{
    QString icon;
    if (hasWindow() && m_currentActiveWindow) {
        icon = m_currentActiveWindow->icon();
    }
    return icon.isEmpty() ? desktopIcon() : icon;
}

bool AppItem::active()
{
    return m_isActive;
}

void AppItem::setActive(bool active)
{
    if (active != m_isActive) {
        m_isActive = active;
        Q_EMIT activeChanged();
    }
}

bool AppItem::isDocked()
{
    return m_isDocked;
}

void AppItem::setDocked(bool docked)
{
    if (docked != m_isDocked) {
        m_isDocked = docked;
        Q_EMIT dockedChanged();
    }
}

AppItem::Style AppItem::itemStyle()
{
    return m_itemStyle;
}

void AppItem::setItemStyle(Style itemStyle)
{
    if (m_itemStyle != itemStyle) {
        m_itemStyle = itemStyle;
        Q_EMIT itemStyleChanged();
    }
}

bool AppItem::hasWindow()
{
    return m_windows.size() > 0;
}

void AppItem::appendWindow(AbstractWindow *window)
{
    m_windows.append(window);
    updateCurrentActiveWindow(window);
}

void AppItem::updateCurrentActiveWindow(AbstractWindow *window)
{
    if (m_currentActiveWindow) {
        disconnect(m_currentActiveWindow, &AbstractWindow::iconChanged, this, &AppItem::iconChanged);
    }
    m_currentActiveWindow = window;
    connect(m_currentActiveWindow, &AbstractWindow::iconChanged, this, &AppItem::iconChanged);
    Q_EMIT currentActiveWindowChanged();
}

}
DS_END_NAMESPACE
