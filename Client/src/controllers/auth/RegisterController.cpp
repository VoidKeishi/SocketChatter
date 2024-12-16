#include "RegisterController.h"
#include "../network/RequestManager.h"
#include "../network/ResponseManager.h"
#include <QDebug>

RegisterController::RegisterController(QObject *parent) : QObject(parent) {
    ResponseManager *responseManager = ResponseManager::instance();

    connect(responseManager, &ResponseManager::registerResponseReceived,
            this, &RegisterController::onRegisterResponse);
}

void RegisterController::handleRegisterRequest(const QString &username, const QString &password) {
    qDebug() << "Register requested with username:" << username;
    RequestManager requestManager;
    requestManager.sendRegisterRequest(username, password);
}

void RegisterController::onRegisterResponse(bool success, const QString &message) {
    emit registerResult(success, message);
}