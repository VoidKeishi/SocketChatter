#include "AuthController.h"
#include "../utils/RequestFactory.h"
#include "../utils/HashUtility.h"
#include "../utils/LoggingCategories.h"
#include "../session/UserManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QDebug>

AuthController::AuthController(QObject *parent) : QObject(parent) {
}

void AuthController::handleLoginRequest(const QString &username, const QString &password) {
    qCInfo(authController) << "Login requested with username:" << username;
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createLoginRequest(username, passwordHash);
    emit loginRequest(requestData);
}

void AuthController::handleRegisterRequest(const QString &username, const QString &password) {
    qCInfo(authController) << "Register requested with username:" << username;
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createRegisterRequest(username, passwordHash);
    emit registerRequest(requestData);
}

void AuthController::onLoginResponse(const QJsonObject &response) {
    QJsonObject payload = response.value("payload").toObject();
    bool success = payload.value("success").toBool();
    if (success) {
        QString username = payload.value("username").toString();
        qCInfo(authController) << "Login successful for user:" << username;
        UserManager::instance()->setCurrentUser(username);
    }
    QString message = payload.value("message").toString(); 
    qCInfo(authController) << "Login response received with success:" << success << ", message:" << message;
    emit loginResult(success, message);
}

void AuthController::onRegisterResponse(const QJsonObject &response) {
    QJsonObject payload = response.value("payload").toObject();
    bool success = payload.value("success").toBool();
    QString message = payload.value("message").toString();
    qCInfo(authController) << "Register response received with success:" << success << ", message:" << message;
    emit registerResult(success, message);
}

void AuthController::logout() {
    qCInfo(authController) << "Logout requested";
    UserManager::instance()->setCurrentUser("");
    emit logoutRequest();
}