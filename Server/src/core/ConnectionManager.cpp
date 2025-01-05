#include <QThread>

#include "ConnectionManager.h"
#include "ClientHandler.h"
#include "../utils/Logger.h"

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

void ConnectionManager::handleNewConnection(QTcpSocket* clientSocket) {
    QString peerAddress = clientSocket->peerAddress().toString();
    if (peerAddress.startsWith("::ffff:")) {
        peerAddress = peerAddress.mid(7); // Remove the "::ffff:" prefix
    }

    // Create a new thread for the client
    QThread* thread = new QThread();
    ClientHandler* clientHandler = new ClientHandler(clientSocket);

    // Move the client handler and socket to the new thread
    clientHandler->moveToThread(thread);
    clientSocket->setParent(nullptr);
    clientSocket->moveToThread(thread);

    // Connect thread lifecycle signals
    connect(thread, &QThread::started, clientHandler, &ClientHandler::start);
    connect(clientSocket, &QTcpSocket::disconnected, clientHandler, &ClientHandler::cleanup);
    connect(clientHandler, &ClientHandler::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(clientHandler, &ClientHandler::finished, clientHandler, &ClientHandler::deleteLater);

    // Start the thread
    thread->start();

    Logger::debug("ClientHandler created for " + peerAddress + ":" + QString::number(clientSocket->peerPort()));
}

void ConnectionManager::addClient(const QString& username, ClientHandler* handler) {
    if (!m_clients.contains(username)) {
        m_clients.insert(username, handler);
        Logger::info(QString("Client added: %1").arg(username));
    } else {
        Logger::error(QString("Client %1 already exists").arg(username));
    }
}

void ConnectionManager::removeClient(const QString& username) {
    if (m_clients.contains(username)) {
        m_clients.remove(username);
        Logger::info(QString("Client removed: %1").arg(username));
    } else {
        Logger::error(QString("Attempted to remove non-existent client: %1").arg(username));
    }
}

ClientHandler* ConnectionManager::getClientHandler(const QString& username) const {
    return m_clients.value(username, nullptr);
}

bool ConnectionManager::sendMessageToClient(const QString& toUsername, const QJsonObject& message) {
    ClientHandler* targetClient = getClientHandler(toUsername);
    if (targetClient) {
        targetClient->sendResponse(message);
        Logger::info(QString("Message sent to %1").arg(toUsername));
        return true;
    } else {
        Logger::error(QString("Failed to send message. Client %1 not found").arg(toUsername));
        return false;
    }
}

