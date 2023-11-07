#pragma once

#include "dsglobal.h"

#include <QString>
#include <QVector>
#include <QObject>

DS_BEGIN_NAMESPACE
namespace dock {
class AbstractWindow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(pid_t pid READ pid NOTIFY pidChanged FINAL)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged FINAL)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged FINAL)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged FINAL)
    Q_PROPERTY(bool shouldSkip READ shouldSkip NOTIFY shouldSkipChanged FINAL)

public:
    AbstractWindow(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~AbstractWindow() {};

    virtual pid_t pid() = 0;
    virtual QString icon() = 0;
    virtual QString title() = 0;
    virtual bool isActive() = 0;
    virtual bool shouldSkip() = 0;

    virtual bool isDemandingAttention() = 0;
    virtual void close() = 0;
    virtual void activate() = 0;
    virtual void minimize() = 0;
    virtual bool isMinimized() = 0;
    virtual int64_t getCreatedTime() = 0;
    virtual QString getWindowType() = 0;
    virtual QString getDisplayName() = 0;
    virtual bool allowClose() = 0;
    virtual void update() = 0;
    virtual void killClient() = 0;

Q_SIGNALS:
    void pidChanged();
    void iconChanged();
    void titleChanged();
    void isActiveChanged();
    void shouldSkipChanged();
};
}
DS_END_NAMESPACE
