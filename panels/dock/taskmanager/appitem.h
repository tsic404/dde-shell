#pragma once

#include "dsglobal.h"
#include "abstractwindow.h"

#include <QMap>
#include <QObject>

DS_BEGIN_NAMESPACE
namespace dock {
class AppItem : public QObject
{
    // indetifier
    Q_PROPERTY(QString id READ id)

    // app item property from desktopfile
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString menus READ menus NOTIFY menusChanged)
    Q_PROPERTY(QString genericName READ genericName NOTIFY genericNameChanged)

    /** item will show icon form window first instead of icon defined from desktop file,
      * so mark icon final to perfrom this behaver, desktopIcon to get icon from desktop file
      */
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged FINAL)
    Q_PROPERTY(QString desktopIcon READ desktopIcon NOTIFY desktopIcon)

    // app item status from dock
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool isDocked READ isDocked WRITE setDocked NOTIFY dockedChanged)
    Q_PROPERTY(Style itemStyle READ itemStyle WRITE setItemStyle NOTIFY itemStyleChanged)

public:
    enum Style {
        WindowMergedStyle      = 0,
        windowSplitedStyle     = 1,
    };
    Q_ENUM(Style)

    AppItem(QString id, QObject *parent = nullptr);

    [[nodiscard]] virtual QString icon() final;

    [[nodiscard]] bool active();
    void setActive(bool active);

    [[nodiscard]] bool isDocked();
    void setDocked(bool docked);

    [[nodiscard]] Style itemStyle();
    void setItemStyle(Style itemStyle);

    void appendWindow(AbstractWindow* window);
    void updateCurrentActiveWindow(AbstractWindow* window);

public:
    [[nodiscard]] virtual QString id();
    [[nodiscard]] virtual QString name();
    [[nodiscard]] virtual QString menus();
    [[nodiscard]] virtual QString genericName();
    [[nodiscard]] virtual QString desktopIcon();

    virtual void handleMenu();
    virtual void launch();
    virtual void launchOptions();
    virtual void requestQuit();

protected:
    [[nodiscard]] bool hasWindow();

Q_SIGNALS:
    void nameChanged();
    void iconChanged();
    void menusChanged();
    void genericNameChanged();

    void activeChanged();
    void dockedChanged();
    void itemStyleChanged();

    void currentActiveWindowChanged();

protected:
    bool m_isActive;
    bool m_isDocked;
    Style m_itemStyle;

    QList<AbstractWindow*> m_windows;
    AbstractWindow* m_currentActiveWindow;
};

using AppItems = QMap<QString, AppItem>;
}
DS_END_NAMESPACE
