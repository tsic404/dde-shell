#pragma once

#include "abstracttrayprotocol.h"

#include <QWidget>
#include <QPointer>
#include <qt6/QtCore/qcoreevent.h>

namespace tray {
class AbstractTrayProtocolHandler;
class TrayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrayWidget(QPointer<AbstractTrayProtocolHandler> handler);
    ~TrayWidget();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPointer<AbstractTrayProtocolHandler> m_handler;
    QTimer *m_attentionTimer;
};
}