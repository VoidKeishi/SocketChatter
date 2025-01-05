#include "AuthViewModel.h"
#include "../controllers/utils/Logger.h"

AuthViewModel::AuthViewModel(QObject* parent)
    : QObject(parent)
    , m_loggedIn(false)
    , m_registered(false)
    , m_currentTab(0)
{
}

void AuthViewModel::setRegistered(bool registered) {
    if (m_registered != registered) {
        m_registered = registered;
        emit registeredChanged();
    }
}

void AuthViewModel::setLoggedIn(bool loggedIn) {
    if (m_loggedIn != loggedIn) {
        m_loggedIn = loggedIn;
        emit loggedInChanged();
    }
}

void AuthViewModel::setMessage(const QString& message) {
    if (m_message != message) {
        m_message = message;
        emit messageChanged();
    }
}

void AuthViewModel::setCurrentTab(int tab) {
    if (m_currentTab != tab) {
        m_currentTab = tab;
        emit currentTabChanged();
    }
}

void AuthViewModel::reset() {
    setRegistered(false);
    setLoggedIn(false);
    setMessage("");
    setCurrentTab(0);
}

void AuthViewModel::sendRequestLogin(const QString& username, const QString& password) {
    Logger::debug("Sending login request for user: " + username);
    emit authActionRequested(AuthAction::Login, username, password);
}

void AuthViewModel::sendRequestRegister(const QString& username, const QString& password) {
    Logger::debug("Sending register request for user: " + username);
    emit authActionRequested(AuthAction::Register, username, password);
}