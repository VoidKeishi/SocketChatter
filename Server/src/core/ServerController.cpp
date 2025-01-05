#include <QTcpSocket>
#include <QThread>

#include "ServerController.h"
#include "ConnectionManager.h"
#include "../utils/Logger.h"

ServerController::ServerController(QObject* parent)
    : QObject(parent)
    , serverSocket(new QTcpServer(this)) 
{
    connect(serverSocket, &QTcpServer::newConnection,
            this, &ServerController::onNewConnection);
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
        
        ConnectionManager::instance()->handleNewConnection(clientSocket);
    }
}
