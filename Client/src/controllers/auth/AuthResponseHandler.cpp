#include "AuthResponseHandler.h"
#include "../session/UserManager.h"
#include "../utils/Logger.h"
#include <QDebug>

AuthResponseHandler::AuthResponseHandler(AuthViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
}

void AuthResponseHandler::handleLoginResponse(const QJsonObject& response) {
    Logger::json("Handling login response", response);
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("username").toString();
        qDebug() << "Setting current user:" << username;
        UserManager::instance()->setCurrentUser(username);
        m_viewModel->setLoggedIn(true);
    }
    
    m_viewModel->setMessage(message);
}

void AuthResponseHandler::handleRegisterResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    m_viewModel->setMessage(message);
    m_viewModel->setRegistered(success);
}