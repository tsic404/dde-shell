#include "dsglobal.h"
#include "docksettingsdconfig.h"
#include "dockabstractsettingsconfig.h"

#include <QObject>
#include <DConfig>
#include <QVariant>

DS_BEGIN_NAMESPACE
namespace dock {
DockDconfig::DockDconfig(QObject* parent)
    : m_dconfig(DConfig::create("dde-dock", "com.deepin.dde.dock", QString(),this))
{
    connect(m_dconfig.data(), &DConfig::valueChanged, this, &DockAbstractConfig::valueChanged);
}

QVariant DockDconfig::value(const QString &key)
{
    if (isValid())
        return m_dconfig->value(key);
    return QVariant();
}

void DockDconfig::setValue(const QString &key, const QVariant &value)
{
    if (isValid())
        m_dconfig->setValue(key, value);
}

bool DockDconfig::isValid()
{
    return m_dconfig && m_dconfig->isValid();
}

}
DS_END_NAMESPACE
