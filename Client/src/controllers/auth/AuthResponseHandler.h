#pragma once

#include <QObject>
#include <QJsonObject>

#include "../../viewmodels/AuthViewModel.h"

class AuthViewModel;

class AuthResponseHandler : public QObject
{
    Q_OBJECT

public:
    explicit AuthResponseHandler(AuthViewModel* viewModel, QObject* parent = nullptr);

public slots:
    void handleLoginResponse(const QJsonObject& response);
    void handleRegisterResponse(const QJsonObject& response);

private:
    AuthViewModel* m_viewModel;
};