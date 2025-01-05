#include "AuthRequestSender.h"
#include "../utils/RequestFactory.h"
#include "../utils/HashUtility.h"
#include "../utils/Logger.h"
#include "../network/NetworkController.h"

AuthRequestSender::AuthRequestSender(QObject* parent)
    : QObject(parent)
{
    connect(this, &AuthRequestSender::sendRequest,
                NetworkController::instance(), &NetworkController::sendData);
}

void AuthRequestSender::requestLogin(const QString& username, const QString& password) {
    Logger::debug("Login requested for user: " + username);
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createLoginRequest(username, passwordHash);
    Logger::debug("Emitting login request");
    emit sendRequest(requestData);
}

void AuthRequestSender::requestRegister(const QString& username, const QString& password) {
    Logger::debug("Register requested for user: " + username);
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createRegisterRequest(username, passwordHash);
    Logger::debug("Emitting register request");
    emit sendRequest(requestData);
}