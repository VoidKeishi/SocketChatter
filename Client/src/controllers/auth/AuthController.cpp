#include "AuthController.h"
#include "../utils/RequestFactory.h"
#include "../utils/HashUtility.h"
#include "../session/UserManager.h"
#include <QDebug>

AuthController::AuthController(AuthViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
    handlers = {
        {"LOGIN_RESPONSE", [this](const QJsonObject& p) { handleLoginResponse(p); }},
        {"REGISTER_RESPONSE", [this](const QJsonObject& p) { handleRegisterResponse(p); }}
    };
}

bool AuthController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

void AuthController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        m_viewModel->setLoading(true);
        handlers[type](payload);
    }
}

void AuthController::requestLogin(const QString& username, const QString& password) {
    qDebug() << "Login requested for user:" << username;
    m_viewModel->setLoading(true);
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createLoginRequest(username, passwordHash);
    qDebug() << "Emitting login request";
    emit sendRequest(requestData);
}

void AuthController::requestRegister(const QString& username, const QString& password) {
    qDebug() << "Register requested for user:" << username;
    m_viewModel->setLoading(true);
    QString passwordHash = HashUtility::hashPassword(password);
    QByteArray requestData = RequestFactory::createRegisterRequest(username, passwordHash);
    qDebug() << "Emitting register request";
    emit sendRequest(requestData);
}

void AuthController::handleLoginResponse(const QJsonObject& response) {
    qDebug() << "Handling login response:" << response;
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("username").toString();
        qDebug() << "Setting current user:" << username;
        UserManager::instance()->setCurrentUser(username);
        m_viewModel->setLoggedIn(true);
    }
    
    m_viewModel->setMessage(message);
    m_viewModel->setLoading(false);
}

void AuthController::handleRegisterResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    m_viewModel->setMessage(message);
    m_viewModel->setRegistered(success);
    m_viewModel->setLoading(false);
}

void AuthController::logout() {
    UserManager::instance()->setCurrentUser("");
    m_viewModel->setLoggedIn(false);
    m_viewModel->setMessage("");
}