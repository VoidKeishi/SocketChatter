#include <QJsonDocument>
#include <QJsonParseError>

#include "ClientHandler.h"
#include "ConnectionManager.h"
#include "../utils/Logger.h"


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
            Logger::error("Received invalid JSON: " + parseError.errorString());
            continue;
        }

        QJsonObject request = doc.object();
        QString clientIp = clientSocket->peerAddress().toString();
        if (clientIp.startsWith("::ffff:")) {
            clientIp = clientIp.mid(7); // Remove the "::ffff:" prefix
        }
        Logger::json(QString("Received request from %1:%2").arg(clientIp).arg(clientSocket->peerPort()), request);        
        processRequest(request);
    }
}

void ClientHandler::onDisconnected() {
    if (!m_username.isEmpty()) {
        ConnectionManager::instance()->removeClient(m_username);
        Logger::info(QString("User %1 disconnected").arg(m_username));
    }
    clientSocket->deleteLater();
}

void ClientHandler::processRequest(const QJsonObject& request) {
    dispatcher->dispatch(request);
}

void ClientHandler::start() {
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
    Logger::json("Sent response", response);
}

void ClientHandler::onLoginSuccess(const QString& username) {
    m_username = username;
    ConnectionManager::instance()->addClient(username, this);
    Logger::info(QString("User %1 connected").arg(username));
}
