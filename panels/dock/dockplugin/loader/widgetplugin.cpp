// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "constants.h"
#include "dockplugin.h"
#include "widgetplugin.h"
#include "pluginsiteminterface.h"

#include <QPainter>
#include <QProcess>
#include <QVBoxLayout>
#include <QMouseEvent>

namespace dock {
WidgetPlugin::WidgetPlugin(PluginsItemInterface* pluginItem)
    : QObject()
    , m_pluginItem(pluginItem)
{
    QMetaObject::invokeMethod(this, [this](){
        m_pluginItem->init(this);
    });
}

WidgetPlugin::~WidgetPlugin()
{
}

void WidgetPlugin::itemAdded(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    DockPlugin* plugin;

    if (m_pluginItem->flags() & Type_Common) {
        auto trayWidget = getQucikPluginTrayWidget(itemKey);
        if (trayWidget == nullptr) {
            trayWidget = m_pluginItem->itemWidget(itemKey);
        }

        if (trayWidget) {
            trayWidget->setAttribute(Qt::WA_TranslucentBackground);
            plugin = getPlugin(trayWidget);
            plugin->setItemKey(itemKey);
            plugin->setPluginType(DockPlugin::Tray);
            plugin->setPluginFlags(m_pluginItem->flags());
            plugin->setPluginId(m_pluginItem->pluginName());
            plugin->setContextMenu(m_pluginItem->itemContextMenu(itemKey));
            trayWidget->show();
        }

        auto quickWidget = m_pluginItem->itemWidget(QUICK_ITEM_KEY);
        if (quickWidget) {
            quickWidget->setAttribute(Qt::WA_TranslucentBackground);
            plugin = getPlugin(quickWidget);
            plugin->setItemKey(itemKey);
            plugin->setPluginType(DockPlugin::Quick);
            plugin->setPluginFlags(m_pluginItem->flags());
            plugin->setPluginId(m_pluginItem->pluginName());
            plugin->setContextMenu(m_pluginItem->itemContextMenu(itemKey));
            quickWidget->show();
        }
    } else {
        auto widget = m_pluginItem->itemWidget(itemKey);
        if (widget) {
            widget->setAttribute(Qt::WA_TranslucentBackground);
            plugin = getPlugin(widget);
            plugin->setItemKey(itemKey);
            plugin->setPluginId(m_pluginItem->pluginName());
            plugin->setPluginFlags(m_pluginItem->flags());
            plugin->setContextMenu(m_pluginItem->itemContextMenu(itemKey));

            DockPlugin::PluginType type;
            if (m_pluginItem->flags() & Type_Fixed) {
                type = DockPlugin::Fixed;
            } else if (m_pluginItem->flags() & Type_System) {
                type = DockPlugin::System;
            } else if (m_pluginItem->flags() & Type_Tool) {
                type = DockPlugin::Tool;
            } else if (m_pluginItem->flags() & Type_Tray) {
                type = DockPlugin::Tray;
            }

            plugin->setPluginType(type);
            widget->show();
        }
    }
}

void WidgetPlugin::itemUpdate(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    if(m_widget) m_widget->update();

    auto widget = m_pluginItem->itemWidget(itemKey);
    if (widget) widget->update();

    auto quickPanel = m_pluginItem->itemWidget(QUICK_ITEM_KEY);
    if(quickPanel) quickPanel->update();

    auto popupWidget = m_pluginItem->itemPopupApplet(itemKey);
    if (popupWidget) popupWidget->update();

    auto tipsWidget = m_pluginItem->itemTipsWidget(itemKey);
    if (tipsWidget) tipsWidget->update();

}
void WidgetPlugin::itemRemoved(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    auto widget = m_pluginItem->itemWidget(itemKey);
    if(widget) widget->hide();

    auto quickPanel = m_pluginItem->itemWidget(QUICK_ITEM_KEY);
    if(quickPanel) quickPanel->hide();

    auto popupWidget = m_pluginItem->itemPopupApplet(itemKey);
    if(popupWidget) popupWidget->hide();

    auto tipsWidget = m_pluginItem->itemTipsWidget(itemKey);
    if(tipsWidget) tipsWidget->hide();

}

void WidgetPlugin::requestWindowAutoHide(PluginsItemInterface * const itemInter, const QString &itemKey, const bool autoHide)
{
}

void WidgetPlugin::requestRefreshWindowVisible(PluginsItemInterface * const itemInter, const QString &itemKey)
{
}

void WidgetPlugin::requestSetAppletVisible(PluginsItemInterface * const itemInter, const QString &itemKey, const bool visible)
{
}

void WidgetPlugin::saveValue(PluginsItemInterface * const itemInter, const QString &key, const QVariant &value)
{
}

const QVariant WidgetPlugin::getValue(PluginsItemInterface *const itemInter, const QString &key, const QVariant& fallback)
{
    return fallback;
}

void WidgetPlugin::removeValue(PluginsItemInterface *const itemInter, const QStringList &keyList)
{
}

void WidgetPlugin::updateDockInfo(PluginsItemInterface *const, const DockPart &part)
{
    switch (part) {
        case DockPart::QuickShow: {
            if (m_widget) {
                m_widget->update();

                auto plugin = getPlugin(m_widget.get());
                plugin->setContextMenu(m_pluginItem->itemContextMenu(plugin->itemKey()));
            }
            break;
        }

        // TODO: implement below cases
        case DockPart::QuickPanel: {
            if (m_widget) {
                m_widget->update();
            }
            break;
        }

        case DockPart::SystemPanel: {
            break;
        }

        case DockPart::DCCSetting: {
            break;
        }
    }
}


const QString WidgetPlugin::pluginName() const
{
    return m_pluginItem->pluginName();
}

const QString WidgetPlugin::itemCommand(const QString &itemKey)
{
    return m_pluginItem->itemCommand(itemKey);
}

const QString WidgetPlugin::itemContextMenu(const QString &itemKey)
{
    return m_pluginItem->itemContextMenu(itemKey);
}

void WidgetPlugin::onDockPositionChanged(uint32_t position)
{
    qApp->setProperty(PROP_POSITION, position);
    m_pluginItem->positionChanged(static_cast<Dock::Position>(position));
}

void WidgetPlugin::onDockDisplayModeChanged(uint32_t displayMode)
{
    qApp->setProperty(PROP_DISPLAY_MODE, displayMode);
    m_pluginItem->displayModeChanged(static_cast<Dock::DisplayMode>(displayMode));
}

QWidget* WidgetPlugin::getQucikPluginTrayWidget(const QString &itemKey)
{
    auto trayIcon = m_pluginItem->icon(DockPart::QuickShow);
    if (trayIcon.isNull())
        return m_widget.get();

    if (m_widget.isNull()) {
        m_widget.reset(new TrayIconWidget(m_pluginItem, itemKey));

        connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [this, itemKey](){
            auto widget = getQucikPluginTrayWidget(itemKey);
            if (widget) widget->update();
        }, Qt::UniqueConnection);
    }

    return m_widget.get();
}

DockPlugin* WidgetPlugin::getPlugin(QWidget* widget)
{
    widget->setParent(nullptr);
    widget->winId();
    auto plugin = DockPlugin::get(widget->windowHandle());
    if (plugin) {
        connect(plugin, &DockPlugin::dockColorThemeChanged, this, [](uint32_t type){
            DGuiApplicationHelper::instance()->setPaletteType(static_cast<DGuiApplicationHelper::ColorType>(type));
        }, Qt::UniqueConnection);

        connect(plugin, &DockPlugin::dockPositionChanged, this, [this](uint32_t position){
            qApp->setProperty(PROP_POSITION, position);
            m_pluginItem->positionChanged(static_cast<Dock::Position>(position));
        }, Qt::UniqueConnection);

        connect(plugin, &DockPlugin::dockDisplayModeChanged, this, [this](uint32_t displayMode){
            qApp->setProperty(PROP_DISPLAY_MODE, displayMode);
            m_pluginItem->displayModeChanged(static_cast<Dock::DisplayMode>(displayMode));
        }, Qt::UniqueConnection);
    }

    return plugin;
}

TrayIconWidget::TrayIconWidget(PluginsItemInterface* pluginItem, QString itemKey, QWidget* parent)
    : QWidget(parent)
    , m_pluginItem(pluginItem)
    , m_itemKey(itemKey)
{
    auto scale = QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps) ? 1 : qApp->devicePixelRatio();
    setFixedSize(PLUGIN_ICON_MIN_SIZE * scale, PLUGIN_ICON_MIN_SIZE * scale);
}

TrayIconWidget::~TrayIconWidget()
{}

void TrayIconWidget::paintEvent(QPaintEvent *event)
{
    auto func = [this]() -> QPixmap {
        auto trayIcon = m_pluginItem->icon(DockPart::QuickShow);
        if (trayIcon.availableSizes().size() > 0) {
            QSize size = trayIcon.availableSizes().first();
            return trayIcon.pixmap(size);
        }

        int pixmapWidth = static_cast<int>(PLUGIN_ICON_MIN_SIZE * (QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps) ? 1 : qApp->devicePixelRatio()));
        int pixmapHeight = static_cast<int>(PLUGIN_ICON_MIN_SIZE * (QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps) ? 1 : qApp->devicePixelRatio()));
        return trayIcon.pixmap(pixmapWidth, pixmapHeight);
    };

    auto pixmap = func();
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());

    QPainter painter(this);
    auto scale = QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps) ? 1 : qApp->devicePixelRatio();
    QSize size = QCoreApplication::testAttribute(Qt::AA_UseHighDpiPixmaps) ? pixmap.size() / qApp->devicePixelRatio() : pixmap.size();
    QRect pixmapRect = QRect(QPoint(0, 0), QSize(PLUGIN_ICON_MIN_SIZE * scale, PLUGIN_ICON_MIN_SIZE * scale));
    painter.drawPixmap(pixmapRect, pixmap);
}

void TrayIconWidget::enterEvent(QEvent *event)
{
    auto popup = m_pluginItem->itemPopupApplet(m_itemKey);
    if (!popup) {
        popup = m_pluginItem->itemPopupApplet(QUICK_ITEM_KEY);
    }

    if (popup && popup->windowHandle()) {
        popup->windowHandle()->hide();
    }

    auto tooltip = m_pluginItem->itemTipsWidget(m_itemKey);
    tooltip->setAttribute(Qt::WA_TranslucentBackground);
    tooltip->setParent(nullptr);
    tooltip->winId();
    
    auto plugin = DockPlugin::get(tooltip->windowHandle());
    plugin->setPluginType(dock::DockPlugin::PluginType::Tooltip);
    plugin->setItemKey(m_itemKey);
    plugin->setPluginId(m_pluginItem->pluginName());
    tooltip->show();
}

void TrayIconWidget::leaveEvent(QEvent *event)
{
    auto tooltip = m_pluginItem->itemTipsWidget(m_itemKey);
    if (tooltip && tooltip->windowHandle())
        tooltip->windowHandle()->hide();
}

void TrayIconWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_pluginItem->itemTipsWidget(m_itemKey)->setVisible(false);

    if (auto popup = m_pluginItem->itemPopupApplet(m_itemKey)) {
        popup->setParent(nullptr);
        popup->setAttribute(Qt::WA_TranslucentBackground);
        popup->winId();
        auto plugin = DockPlugin::get(popup->windowHandle());
        plugin->setPluginType(dock::DockPlugin::PluginType::Tooltip);
        plugin->setItemKey(m_itemKey);
        plugin->setPluginId(m_pluginItem->pluginName());
        popup->show();
    } else if (auto popup = m_pluginItem->itemPopupApplet(QUICK_ITEM_KEY)) {
        popup->setParent(nullptr);
        popup->setAttribute(Qt::WA_TranslucentBackground);
        popup->winId();
        auto plugin = DockPlugin::get(popup->windowHandle());
        plugin->setPluginType(dock::DockPlugin::PluginType::Tooltip);
        plugin->setItemKey(m_itemKey);
        plugin->setPluginId(m_pluginItem->pluginName());
        popup->show();
    } else {
        qDebug() << "no popup";
        QStringList commandArgument = m_pluginItem->itemCommand(m_itemKey).split(" ");
        if (commandArgument.size() > 0) {
            QString command = commandArgument.first();
            commandArgument.removeFirst();
            QProcess::startDetached(command, commandArgument);
        }
    }
}

}
