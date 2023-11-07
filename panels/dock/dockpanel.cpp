#include "panel.h"
#include "dsglobal.h"
#include "constants.h"
#include "dockpanel.h"
#include "dockadaptor.h"
#include "docksettings.h"
#include "pluginfactory.h"

// for old api compatible
#include "dockdbusproxy.h"
#include "dockfrontadaptor.h"
#include "dockdaemonadaptor.h"

#include <QQuickWindow>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(dockLog, "dde.shell.dock")

DS_BEGIN_NAMESPACE
namespace dock {

DockPanel::DockPanel(QObject * parent)
    : DPanel(parent)
{
}

void DockPanel::load()
{
    DPanel::load();
}

void DockPanel::init()
{
    DockAdaptor* adaptor = new DockAdaptor(this);
    QDBusConnection::sessionBus().registerService("org.deepin.ds.Dock");
    QDBusConnection::sessionBus().registerObject("/org/deepin/ds/Dock", "org.deepin.ds.Dock", this);

    // for old api compatible
    DockDBusProxy* proxy = new DockDBusProxy(this);
    DockFrontAdaptor* dockFrontAdaptor = new DockFrontAdaptor(proxy);
    QDBusConnection::sessionBus().registerService("org.deepin.dde.Dock1");
    QDBusConnection::sessionBus().registerObject("/org/deepin/dde/Dock1", "org.deepin.dde.Dock1", proxy);

    DockDaemonAdaptor* dockDaemonAdaptor = new DockDaemonAdaptor(proxy);
    QDBusConnection::sessionBus().registerService("org.deepin.dde.daemon.Dock1");
    QDBusConnection::sessionBus().registerObject("/org/deepin/dde/daemon/Dock1", "org.deepin.dde.daemon.Dock1", proxy);
    connect(DockSettings::instance(), &DockSettings::positionChanged, this, [this](){
        Q_EMIT positionChanged(position());
    });
    
    DPanel::init();
    // those connections need connect after DPanel::init() which create QQuickWindow
    connect(window(), &QQuickWindow::xChanged, this, &DockPanel::onWindowGeometryChanged);
    connect(window(), &QQuickWindow::yChanged, this, &DockPanel::onWindowGeometryChanged);
    connect(window(), &QQuickWindow::widthChanged, this, &DockPanel::onWindowGeometryChanged);
    connect(window(), &QQuickWindow::heightChanged, this, &DockPanel::onWindowGeometryChanged);
    QMetaObject::invokeMethod(this, &DockPanel::onWindowGeometryChanged);
}

QRect DockPanel::geometry()
{
    return window()->geometry();
}

QRect DockPanel::frontendWindowRect()
{
    auto ratio = window()->devicePixelRatio();
    auto screenGeometry = window()->screen()->geometry();
    auto geometry = window()->geometry();
    auto x = geometry.x(), y = geometry.y();
    switch (position()) {
        case Top: {
            break;
        }
        case Bottom: {
            y += screenGeometry.height() - geometry.height();
            break;
        }
        case Left: {
            break;
        }
        case Right: {
            x += screenGeometry.width() - geometry.width();
            break;
        }
    }
    return QRect(x, y, geometry.width() * ratio, geometry.height() * ratio);
}

Position DockPanel::position()
{
    return DockSettings::instance()->position();
}

void DockPanel::setPosition(Position position)
{
    DockSettings::instance()->setPosition(position);
}

HideMode DockPanel::hideMode()
{
    return DockSettings::instance()->hideMode();
}

void DockPanel::setHideMode(HideMode mode)
{
    DockSettings::instance()->setHideMode(mode);
}

DisplayMode DockPanel::displayMode()
{
    return DockSettings::instance()->displayMode();
}

void DockPanel::setDisplayMode(DisplayMode mode)
{
    DockSettings::instance()->setDisplayMode(mode);
}

uint DockPanel::dockSize()
{
    switch (displayMode()) {
        case Fashion:
            return DockSettings::instance()->windowSizeFashion();
        case Efficient:
            return DockSettings::instance()->windowSizeEfficient();
    }
    qCWarning(dockLog()) << "unknown display mode return MIN_DOCK_SIZE";
    return MIN_DOCK_SIZE;
}

void DockPanel::setDockSize(uint size)
{
    switch (displayMode()) {
        case Fashion: {
            DockSettings::instance()->setWindowSizeFashion(size);
            return;
        }
        case Efficient: {
            DockSettings::instance()->setWindowSizeEfficient(size);
        }
    }
    qCWarning(dockLog()) << "unknown display mode not set dock size to dock settings";
}

void DockPanel::onWindowGeometryChanged()
{
    Q_EMIT frontendWindowRectChanged(frontendWindowRect());
    Q_EMIT geometryChanged(geometry());
}

D_APPLET_CLASS(DockPanel)

}

DS_END_NAMESPACE
#include "dockpanel.moc"
