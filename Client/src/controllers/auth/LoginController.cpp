#include "LoginController.h"
#include "../network/RequestManager.h"
#include "../network/ResponseManager.h"
#include <QDebug>

LoginController::LoginController(QObject *parent) : QObject(parent) {
    ResponseManager *responseManager = ResponseManager::instance();
    connect(responseManager, &ResponseManager::loginResponseReceived,
            this, &LoginController::onLoginResponse);
}

void LoginController::handleLoginRequest(const QString &username, const QString &password) {
    qDebug() << "Login requested with username:" << username;
    RequestManager requestManager;
    requestManager.sendLoginRequest(username, password);
}

void LoginController::onLoginResponse(bool success, const QString &message) {
    emit loginResult(success, message);
}