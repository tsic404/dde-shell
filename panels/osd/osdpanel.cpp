// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "osdpanel.h"

#include "pluginfactory.h"

#include <QDBusConnection>
#include <QTimer>
#include <QLoggingCategory>
#include <QDBusConnection>
#include <QDBusError>

DS_BEGIN_NAMESPACE
namespace osd {

OsdPanel::OsdPanel(QObject *parent)
    : DPanel(parent)
{
}

void OsdPanel::load()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService("org.deepin.dde.Shell")) {
        qWarning() << "register failed" << bus.lastError().message();
    }

    DPanel::load();
}

void OsdPanel::init()
{
    auto bus = QDBusConnection::sessionBus();
    bus.registerObject(QStringLiteral("/org/deepin/osdService"),
                       this,
                       QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);

    m_osdTimer = new QTimer(this);
    m_osdTimer->setInterval(m_interval);
    m_osdTimer->setSingleShot(true);
    QObject::connect(m_osdTimer, &QTimer::timeout, this, &OsdPanel::hideOsd);
    DPanel::init();
}

Q_LOGGING_CATEGORY(osdLog, "dde.shell.osd")

QString OsdPanel::osdType() const
{
    return m_osdType;
}

bool OsdPanel::visible() const
{
    return m_visible;
}

void OsdPanel::showText(const QString &text)
{
    qCInfo(osdLog()) << "show text" << text;
    m_osdTimer->setInterval(text == "SwitchWM3D" ? 2000 : 1000);

    setOsdType(text);
    showOsd();
}

void OsdPanel::hideOsd()
{
    m_osdTimer->stop();
    setVisible(false);
}

void OsdPanel::showOsd()
{
    m_osdTimer->stop();

    m_osdTimer->start();
    setVisible(true);
}

void OsdPanel::setVisible(const bool visible)
{
    if (visible == m_visible)
        return;
    m_visible = visible;
    Q_EMIT visibleChanged();
}

void OsdPanel::setOsdType(const QString &osdType)
{
    m_osdType = osdType;
    emit osdTypeChanged(m_osdType);
}

D_APPLET_CLASS(OsdPanel)

}
DS_END_NAMESPACE

#include "osdpanel.moc"
