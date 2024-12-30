#pragma once
#include "../IController.h"
#include "../../viewmodels/AuthViewModel.h"
#include <QObject>
#include <QMap>
#include <functional>

class AuthController : public QObject, public IController {
    Q_OBJECT
public:
    explicit AuthController(AuthViewModel* viewModel, QObject* parent = nullptr);
    
    // IController interface
    bool canHandle(const QString& type) const override;
    void handle(const QString& type, const QJsonObject& payload) override;

public slots:
    void requestLogin(const QString& username, const QString& password);
    void requestRegister(const QString& username, const QString& password);
    void logout();

signals:
    void sendRequest(const QByteArray& data);

private:
    AuthViewModel* m_viewModel;
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;

    void handleLoginResponse(const QJsonObject& response);
    void handleRegisterResponse(const QJsonObject& response);
};