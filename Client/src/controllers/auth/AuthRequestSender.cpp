#include <QDebug>

#include "AuthRequestSender.h"
#include "../utils/RequestFactory.h"
#include "../utils/HashUtility.h"
#include "../network/NetworkController.h"

AuthRequestSender::AuthRequestSender(QObject* parent)
    : QObject(parent)
{
    connect(this, &AuthRequestSender::sendRequest,
                NetworkController::instance(), &NetworkController::sendData);
}

void AuthRequestSender::requestLogin(const QString& username, const QString& password) {
    qDebug() << "Login requested for user:" << username;
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createLoginRequest(username, passwordHash);
    qDebug() << "Emitting login request";
    emit sendRequest(requestData);
}

void AuthRequestSender::requestRegister(const QString& username, const QString& password) {
    qDebug() << "Register requested for user:" << username;
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createRegisterRequest(username, passwordHash);
    qDebug() << "Emitting register request";
    emit sendRequest(requestData);
}