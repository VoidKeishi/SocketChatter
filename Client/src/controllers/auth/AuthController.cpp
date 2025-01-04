#include <QDebug>

#include "AuthController.h"
#include "../utils/RequestFactory.h"
#include "../utils/HashUtility.h"
#include "../session/UserManager.h"
#include "../utils/Logger.h"

AuthController::AuthController(AuthViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
    , m_requestSender(new AuthRequestSender(this))
    , m_responseHandler(new AuthResponseHandler(viewModel, this))
{
    handlers = {
        {"LOGIN_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleLoginResponse(p); }},
        {"REGISTER_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleRegisterResponse(p); }}
    };
    connect(viewModel, &AuthViewModel::authActionRequested,
        this, &AuthController::handleAuthAction);
}

void AuthController::handleAuthAction(AuthAction action, const QString& username, const QString& password) {
    switch (action) {
    case AuthAction::Register:
        m_requestSender->requestRegister(username, password);
        break;
    case AuthAction::Login:
        m_requestSender->requestLogin(username, password);
        break;
    }
}

bool AuthController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

void AuthController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        handlers[type](payload);
    }
}