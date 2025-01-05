#include <QThread>

#include "ServerController.h"
#include "ClientHandler.h"
#include "../database/DatabaseManager.h"
#include "../utils/Logger.h"

ServerController::ServerController(QObject* parent)
    : QObject(parent), serverSocket(new QTcpServer(this)) {
    connect(serverSocket, &QTcpServer::newConnection, this, &ServerController::onNewConnection);
}

void ServerController::startServer(quint16 port) {
    DatabaseManager::instance()->init();

    if (!serverSocket->listen(QHostAddress::Any, port)) {
        Logger::error("Server could not start!");
    } else {
        Logger::info("Server started on port " + QString::number(port));
    }
}

void ServerController::onNewConnection() {
    while (serverSocket->hasPendingConnections()) {
        QTcpSocket* clientSocket = serverSocket->nextPendingConnection();
        QString peerAddress = clientSocket->peerAddress().toString();
        if (peerAddress.startsWith("::ffff:")) {
            peerAddress = peerAddress.mid(7); // Remove the "::ffff:" prefix
        }
        Logger::debug("Received connection from " + peerAddress + ":" + QString::number(clientSocket->peerPort()));
        
        QThread* thread = new QThread();
        ClientHandler* clientHandler = new ClientHandler(clientSocket);
        clientSocket->setParent(nullptr);
        clientHandler->moveToThread(thread);
        clientSocket->moveToThread(thread);
        
        connect(thread, &QThread::started, clientHandler, &ClientHandler::start);
        connect(clientSocket, &QTcpSocket::disconnected, clientHandler, &ClientHandler::cleanup);
        connect(clientHandler, &ClientHandler::finished, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(clientHandler, &ClientHandler::finished, clientHandler, &ClientHandler::deleteLater);
        
        // pass ip address to client handler
        thread->start();
    }
}

void ServerController::addClient(const QString& username, ClientHandler* client) {
    clients.insert(username, client);
    emit clientAdded(username);
}

void ServerController::removeClient(const QString& username) {
    if (clients.contains(username)) {
        clients.remove(username);
        emit clientRemoved(username);
    }
}

ClientHandler* ServerController::getClientHandler(const QString& username) const {
    return clients.value(username, nullptr);
}
