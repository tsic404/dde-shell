#pragma once


#include "appitem.h"
#include "dsglobal.h"
#include "applicationinterface.h"

#include <QJsonArray>
#include <QObject>
#include <QScopedPointer>

using Application = org::desktopspec::ApplicationManager1::Application;

DS_BEGIN_NAMESPACE
namespace dock {

/** this use org.desktopspec.ApplicationManager1 as appitem backend
  * appitem info get form org.desktopspec.ApplicationManager1 dbus
  */

class AppItemAm : public AppItem
{
public:
    AppItemAm(QString id, QObject *parent = nullptr);
    AppItemAm(QDBusObjectPath dbusPath, QObject *parent = nullptr);
    ~AppItemAm();

    virtual void launch() override;
    virtual void launchOptions() override;

    virtual QString id() override;
    virtual QString name() override;
    virtual QString desktopIcon() override;
    virtual QString menus() override;
    virtual QString genericName() override;

private:
    QString dbusPath2id(const QString& dbusPath);
    QString id2dbusPath(const QString& id);

private:
    QString m_id;
    QString m_name;
    QString m_icon;
    QString m_menus;
    QString m_genericName;
    QScopedPointer<Application> m_applicationInterface;
};
}
DS_END_NAMESPACE
