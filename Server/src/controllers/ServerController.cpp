#include "ServerController.h"
#include "ClientHandler.h"
#include <QDebug>

ServerController::ServerController(QObject* parent)
    : QObject(parent), serverSocket(new QTcpServer(this)) {
    connect(serverSocket, &QTcpServer::newConnection, this, &ServerController::onNewConnection);
}

void ServerController::startServer(quint16 port) {
    if (!serverSocket->listen(QHostAddress::Any, port)) {
        qCritical() << "Server could not start!";
    } else {
        qInfo() << "Server started on port" << port;
    }
}

void ServerController::onNewConnection() {
    while (serverSocket->hasPendingConnections()) {
        QTcpSocket* clientSocket = serverSocket->nextPendingConnection();
        QString peerAddress = clientSocket->peerAddress().toString();
        if (peerAddress.startsWith("::ffff:")) {
            peerAddress = peerAddress.mid(7); // Remove the "::ffff:" prefix
        }
        qDebug() << "Received connection from" << peerAddress << ":" << clientSocket->peerPort();
        ClientHandler* clientHandler = new ClientHandler(clientSocket, this);
        connect(clientHandler, &ClientHandler::disconnected, clientHandler, &ClientHandler::deleteLater);
    }
}