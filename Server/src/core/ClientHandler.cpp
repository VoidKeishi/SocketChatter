#include "ClientHandler.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>


ClientHandler::ClientHandler(QTcpSocket* socket, QObject* parent) 
    : QObject(parent)
    , clientSocket(socket)
    , dispatcher(new RequestDispatcher(this)) 
    , authHandler(new AuthHandler(DatabaseManager::instance())) 
    , contactHandler(new ContactHandler(DatabaseManager::instance())) {

    dispatcher->registerHandler(authHandler);
    dispatcher->registerHandler(contactHandler);
    
        
    connect(clientSocket, &QTcpSocket::readyRead, 
            this, &ClientHandler::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, 
            this, &ClientHandler::onDisconnected);
    connect(dispatcher, &RequestDispatcher::responseReady,
            this, [this](const QJsonObject& response) {
        QJsonDocument doc(response);
        clientSocket->write(doc.toJson() + "\n");
    });
    connect(dispatcher, &RequestDispatcher::responseReady,
            this, &ClientHandler::sendResponse);
}

void ClientHandler::onReadyRead() {
    buffer.append(clientSocket->readAll());
    while (true) {
        int endIndex = buffer.indexOf('\n');
        if (endIndex == -1) {
            break; // No complete message yet
        }

        QByteArray data = buffer.left(endIndex);
        buffer.remove(0, endIndex + 1);

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (doc.isNull() || !doc.isObject()) {
            qWarning() << "Received invalid JSON:" << parseError.errorString();
            continue;
        }

        QJsonObject request = doc.object();
        QString clientIp = clientSocket->peerAddress().toString();
        if (clientIp.startsWith("::ffff:")) {
            clientIp = clientIp.mid(7); // Remove the "::ffff:" prefix
        }
        qDebug() << "Received request from" << clientIp << ":" << clientSocket->peerPort() << request;
        processRequest(request);
    }
}

void ClientHandler::onDisconnected() {
    emit disconnected();
}

void ClientHandler::processRequest(const QJsonObject& request) {
    dispatcher->dispatch(request);
}

void ClientHandler::setUsername (const QString& user) {
    username = user;
}

void ClientHandler::start() {
    qDebug() << "New ClientHandler started";
}

void ClientHandler::cleanup() {
    if (clientSocket) {
        clientSocket->disconnectFromHost();
        if (clientSocket->state() != QAbstractSocket::UnconnectedState) {
            clientSocket->waitForDisconnected();
        }
    }
    emit finished();
}

void ClientHandler::sendResponse(const QJsonObject& response) {
    QJsonDocument doc(response);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    clientSocket->write(data);
    clientSocket->flush();
    qDebug() << "Sent response:" << QString(data);
}