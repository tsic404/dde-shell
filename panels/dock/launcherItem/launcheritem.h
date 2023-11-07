#pragma once

#include "applet.h"
#include "dsglobal.h"

DS_BEGIN_NAMESPACE
namespace dock {

class LauncherItem : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged FINAL)
public:
    explicit LauncherItem(QObject *parent = nullptr);
    virtual void init() override;

    QString iconName() const;
    void setIconName(const QString& iconName);

    Q_INVOKABLE void toggleLauncher();

Q_SIGNALS:
    void iconNameChanged();

private:
    QString m_iconName;
};

}
DS_END_NAMESPACE
