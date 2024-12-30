#include "AuthViewModel.h"
#include <QDebug>

AuthViewModel::AuthViewModel(QObject* parent)
    : QObject(parent)
    , m_status(AuthStatus::Idle)
    , m_loggedIn(false)
    , m_registered(false)
    , m_currentTab(0)
{
}

void AuthViewModel::setLoading(bool loading) {
    AuthStatus newStatus = loading ? AuthStatus::Loading : AuthStatus::Idle;
    if (m_status != newStatus) {
        m_status = newStatus;
        emit loadingChanged();
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

void AuthViewModel::setRegistered(bool registered) {
    if (m_registered != registered) {
        m_registered = registered;
        emit registeredChanged();
    }
}

void AuthViewModel::setCurrentTab(int tab) {
    if (m_currentTab != tab) {
        m_currentTab = tab;
        emit currentTabChanged();
    }
}

void AuthViewModel::reset() {
    setLoading(false);
    setLoggedIn(false);
    setMessage("");
    setRegistered(false);
    setCurrentTab(0);
}