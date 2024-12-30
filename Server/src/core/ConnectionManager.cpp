#include "ConnectionManager.h"

ConnectionManager* ConnectionManager::m_instance = nullptr;

ConnectionManager* ConnectionManager::instance() {
    if (!m_instance) {
        m_instance = new ConnectionManager();
    }
    return m_instance;
}

ConnectionManager::ConnectionManager(QObject* parent)
    : QObject(parent) {
}

void ConnectionManager::addClient(const QString& username, ClientHandler* handler) {
    if (!m_clients.contains(username)) {
        m_clients.insert(username, handler);
    }
}

void ConnectionManager::removeClient(const QString& username) {
    m_clients.remove(username);
}

ClientHandler* ConnectionManager::getClientHandler(const QString& username) const {
    return m_clients.value(username, nullptr);
}