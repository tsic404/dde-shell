#pragma once

#include "panel.h"
#include "dsglobal.h"
#include "constants.h"

#include <QDBusContext>

DS_BEGIN_NAMESPACE
namespace dock {

class DockPanel : public DPanel, public QDBusContext
{
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry FINAL)

    Q_PROPERTY(QRect frontendWindowRect READ frontendWindowRect NOTIFY frontendWindowRectChanged FINAL)
    Q_PROPERTY(Position position READ position WRITE setPosition NOTIFY positionChanged FINAL)

    Q_PROPERTY(HideState hideState READ hideState NOTIFY hideStateChanged FINAL)
    Q_PROPERTY(HideMode hideMode READ hideMode WRITE setHideMode NOTIFY hideModeChanged FINAL)
    Q_PROPERTY(DisplayMode displayMode READ displayMode WRITE setDisplayMode NOTIFY displayModeChanged FINAL)
    Q_PROPERTY(uint dockSize READ dockSize WRITE setDockSize NOTIFY dockSizeChanged FINAL)

public:
    explicit DockPanel(QObject *parent = nullptr);

    void load() override;
    void init() override;

    void ReloadPlugins();
    void callShow();

    QRect geometry();
    QRect frontendWindowRect();

    Position position();
    void setPosition(Position position);

    HideMode hideMode();
    void setHideMode(HideMode mode);

    DisplayMode displayMode();
    void setDisplayMode(DisplayMode mode);

    HideState hideState();

    uint dockSize();
    void setDockSize(uint size);

private Q_SLOTS:
    void onWindowGeometryChanged();
    void onQmlRootObjectChanged();

Q_SIGNALS:
    void frontendWindowRectChanged(QRect frontendWindowRect);
    void geometryChanged(QRect geometry);
    void positionChanged(Position position);
    void hideModeChanged(HideMode mode);
    void hideStateChanged(HideState state);
    void displayModeChanged(DisplayMode mode);
    void dockSizeChanged(uint size);

};

}
DS_END_NAMESPACE
