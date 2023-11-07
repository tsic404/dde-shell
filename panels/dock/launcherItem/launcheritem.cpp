#include "applet.h"
#include "launcheritem.h"
#include "pluginfactory.h"

#include <DDBusSender>

DS_BEGIN_NAMESPACE
namespace dock {

static DDBusSender launcherDbus()
{
    return DDBusSender().service("org.deepin.dde.Launcher1")
        .path("/org/deepin/dde/Launcher1")
        .interface("org.deepin.dde.Launcher1");
}

LauncherItem::LauncherItem(QObject *parent)
    : DApplet(parent)
    , m_iconName("deepin-launcher")
{

}

void LauncherItem::init()
{
    DApplet::init();
}

void LauncherItem::toggleLauncher()
{
    launcherDbus().method("Toggle").call();
}

QString LauncherItem::iconName() const
{
    return m_iconName;
}

void LauncherItem::setIconName(const QString& iconName)
{
    if (iconName != m_iconName) {
        m_iconName = iconName;
        Q_EMIT iconNameChanged();
    }
}

D_APPLET_CLASS(LauncherItem)
}

DS_END_NAMESPACE

#include "launcheritem.moc"
