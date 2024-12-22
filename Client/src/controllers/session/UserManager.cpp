#include "UserManager.h"

UserManager* UserManager::m_instance = nullptr;

UserManager* UserManager::instance() {
    if (!m_instance) {
        m_instance = new UserManager();
    }
    return m_instance;
}

UserManager::UserManager(QObject* parent) : QObject(parent) {}

void UserManager::setCurrentUser(const QString& username) {
    m_currentUser = username;
}