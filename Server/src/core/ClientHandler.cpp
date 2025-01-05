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
    , contactHandler(new ContactHandler(DatabaseManager::instance())) 
    , messageHandler(new MessageHandler(DatabaseManager::instance()))

{
    dispatcher->registerHandler(authHandler);
    dispatcher->registerHandler(contactHandler);
    dispatcher->registerHandler(messageHandler);
    
        
    connect(clientSocket, &QTcpSocket::readyRead, 
            this, &ClientHandler::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, 
            this, &ClientHandler::onDisconnected);
    connect(dispatcher, &RequestDispatcher::responseReady,
            this, &ClientHandler::sendResponse);
    connect(authHandler, &AuthHandler::loginSuccess,
            this, &ClientHandler::onLoginSuccess);
    // Connect to ConnectionManager's messageToClient signal
    connect(ConnectionManager::instance(), &ConnectionManager::messageToClient,
            this, &ClientHandler::handleMessage,
            Qt::QueuedConnection); 
}

void ClientHandler::start() {
    QString clientIp = clientSocket->peerAddress().toString();
    if (clientIp.startsWith("::ffff:")) {
        clientIp = clientIp.mid(7); // Remove the "::ffff:" prefix
    }
    Logger::info("ClientHandler started for " + clientIp + ":" + QString::number(clientSocket->peerPort()));
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
            qDebug() << "null: " << doc.isNull(); 
            qDebug() << "not obj: " << doc.isObject(); 
            Logger::error("Received invalid JSON: " + parseError.errorString());
            continue;
        }

        QJsonObject request = doc.object();
        QString clientIp = clientSocket->peerAddress().toString();
        if (clientIp.startsWith("::ffff:")) {
            clientIp = clientIp.mid(7); // Remove the "::ffff:" prefix
        }
        if (!m_username.isEmpty()) {
            Logger::json(QString("Request received from user: %1").arg(m_username), request);
        } else {
            Logger::json(QString("Received request from %1:%2").arg(clientIp).arg(clientSocket->peerPort()), request);    
        }      
        processRequest(request);
    }
}

void ClientHandler::processRequest(const QJsonObject& request) {
    dispatcher->dispatch(request);
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

void ClientHandler::handleMessage(const QString& toUsername, const QJsonObject& message) {
    if (toUsername == m_username) {
        sendResponse(message);
    }
}

void ClientHandler::onLoginSuccess(const QString& username) {
    m_username = username;
    ConnectionManager::instance()->addClient(username, this);
    messageHandler->registerClient(username, clientSocket);
    Logger::info(QString("User %1 connected").arg(username));
}

void ClientHandler::onDisconnected() {
    if (!m_username.isEmpty()) {
        ConnectionManager::instance()->removeClient(m_username);
        messageHandler->removeClient(m_username);
        Logger::info(QString("User %1 disconnected").arg(m_username));
    }
    clientSocket->deleteLater();
}
