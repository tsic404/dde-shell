#include "applet.h"
#include "showdesktop.h"
#include "pluginfactory.h"

#include <QProcess>

DS_BEGIN_NAMESPACE
namespace dock {

ShowDesktop::ShowDesktop(QObject *parent)
    : DApplet(parent)
    , m_iconName("typora")
{

}

void ShowDesktop::init()
{
    DApplet::init();
}

void ShowDesktop::toggleShowDesktop()
{
    QProcess::startDetached("/usr/lib/deepin-daemon/desktop-toggle", QStringList());
}

QString ShowDesktop::iconName() const
{
    return m_iconName;
}

void ShowDesktop::setIconName(const QString& iconName)
{
    if (iconName != m_iconName) {
        m_iconName = iconName;
        Q_EMIT iconNameChanged();
    }
}

D_APPLET_CLASS(ShowDesktop)
}

DS_END_NAMESPACE

#include "showdesktop.moc"
