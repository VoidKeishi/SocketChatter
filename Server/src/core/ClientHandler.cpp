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
    QByteArray newData = clientSocket->readAll();
    Logger::debug(QString("Raw data received (hex): %1").arg(QString(newData.toHex())));
    if (newData.trimmed().isEmpty()) {
        Logger::debug("Skipping empty/whitespace data");
        return;
    }
    buffer.append(newData);

    while (true) {
        int endIndex = buffer.indexOf('\n');
        if (endIndex == -1) {
            break; 
        }

        QByteArray data = buffer.left(endIndex);
        buffer.remove(0, endIndex + 1);

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (doc.isNull() || !doc.isObject()) {
            Logger::debug(QString("Json doc is null: %1").arg(doc.isNull()));
            Logger::debug(QString("Json doc is not object: %1").arg(!doc.isObject()));
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

// void ClientHandler::sendResponse(const QJsonObject& response) {
//     QJsonDocument doc(response);
//     QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
//     clientSocket->write(data);
//     clientSocket->flush();
//     Logger::json("Sent response", response);
// }

void ClientHandler::sendResponse(const QJsonObject& response) {
    QJsonDocument doc(response);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    
    Logger::debug("=== Response Debug ===");
    Logger::debug(QString("Response size before send: %1").arg(data.size()));
    Logger::debug(QString("Socket state: %1").arg(clientSocket->state()));
    Logger::debug(QString("Socket is valid: %1").arg(clientSocket->isValid()));
    Logger::debug(QString("Socket is writable: %1").arg(clientSocket->isWritable()));
    
    qint64 bytesWritten = clientSocket->write(data);
    Logger::debug(QString("Bytes written: %1").arg(bytesWritten));
    
    bool flushSuccess = clientSocket->flush();
    Logger::debug(QString("Flush success: %1").arg(flushSuccess));
    
    Logger::json("Sent response", response);
    Logger::debug("=== End Response Debug ===");
}

void ClientHandler::handleMessage(const QString& toUsername, const QJsonObject& message) {
    if (toUsername == m_username) {
        sendResponse(message);
    }
}

void ClientHandler::onLoginSuccess(const QString& username) {
    m_username = username;
    ConnectionManager::instance()->addClient(username, this);
    Logger::info(QString("User %1 connected").arg(username));
}

void ClientHandler::onDisconnected() {
    if (!m_username.isEmpty()) {
        ConnectionManager::instance()->removeClient(m_username);
        Logger::info(QString("User %1 disconnected").arg(m_username));
    }
    clientSocket->deleteLater();
}
