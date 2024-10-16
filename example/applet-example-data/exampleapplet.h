// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "applet.h"

DS_USE_NAMESPACE

class ExampleApplet : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(QString mainText READ mainText NOTIFY mainTextChanged CONSTANT)
public:
    explicit ExampleApplet(QObject *parent = nullptr);

    QString mainText() const;

    virtual void init() override;

Q_SIGNALS:
    void mainTextChanged();
private:
    QString m_mainText;
};
