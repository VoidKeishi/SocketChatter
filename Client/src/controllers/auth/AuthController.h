#pragma once

#include <QObject>
#include <QMap>
#include <functional>

#include "AuthRequestSender.h"
#include "AuthResponseHandler.h"
#include "../IController.h"
#include "../../viewmodels/AuthViewModel.h"


class AuthController : public QObject, public IController {
    Q_OBJECT
public:
    explicit AuthController(AuthViewModel* viewModel, QObject* parent = nullptr);
    
    // IController interface
    bool canHandle(const QString& type) const override;
    void handle(const QString& type, const QJsonObject& payload) override;

public slots:
    void handleAuthAction(AuthAction action, const QString& username, const QString& password);

private:
    AuthViewModel* m_viewModel;
    AuthRequestSender* m_requestSender;
    AuthResponseHandler* m_responseHandler;
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;

};