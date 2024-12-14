#include "ClientHandler.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>
#include "../database/DatabaseManager.h"

ClientHandler::ClientHandler(QTcpSocket* socket, QObject* parent)
    : QObject(parent), clientSocket(socket) {
    connect(clientSocket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
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
    QString username = DatabaseManager::instance().getUsernameBySocket(clientSocket);
    if (!username.isEmpty()) {
        DatabaseManager::instance().removeUserSocket(username);
    }
    qDebug() << "Client " << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort() << " disconnected";
    emit disconnected();
}

void ClientHandler::processRequest(const QJsonObject& request) {
    QString type = request.value("type").toString();
    QJsonObject payload = request.value("payload").toObject();

    if (type == "LOGIN_REQUEST") {
        handleLoginRequest(payload);
    } else if (type == "REGISTER_REQUEST") {
        handleRegisterRequest(payload);
    } else if (type == "TEXT_MESSAGE") {
        handleMessageSend(payload);
    } else if (type == "FRIEND_REQUEST") {
        handleFriendRequest(payload);
    } else if (type == "FETCH_PENDING_INVITATIONS") {
        handleFetchPendingInvitations(payload);
    } else if (type == "FRIEND_REQUEST_RESPONSE") {
        handleFriendRequestResponse(payload);
    } else if (type == "FETCH_CONTACT_LIST") {
        handleFetchContactList(payload);
    } else {
        qWarning() << "Unknown request type:" << type;
    }
}

void ClientHandler::handleLoginRequest(const QJsonObject& payload) {
    QString username = payload.value("username").toString();
    QString passwordHash = payload.value("password").toString();

    bool success = DatabaseManager::instance().authenticateUser(username, passwordHash);

    QJsonObject response;
    response["type"] = "LOGIN_RESPONSE";
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject responsePayload;
    responsePayload["success"] = success;
    responsePayload["error"] = success ? "" : "Invalid credentials";
    response["payload"] = responsePayload;

    if (success) {
        DatabaseManager::instance().addUserSocket(username, clientSocket);
    }

    sendResponse(response);
}

void ClientHandler::handleRegisterRequest(const QJsonObject& payload) {
    QString username = payload.value("username").toString();
    QString passwordHash = payload.value("password").toString();

    bool success = DatabaseManager::instance().registerUser(username, passwordHash);

    QJsonObject response;
    response["type"] = "REGISTER_RESPONSE";
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject responsePayload;
    responsePayload["success"] = success;
    responsePayload["error"] = success ? "" : "User already exists";
    response["payload"] = responsePayload;

    sendResponse(response);
}

void ClientHandler::handleMessageSend(const QJsonObject& payload) {
    QString sender = payload.value("sender").toString();
    QString receiver = payload.value("receiver").toString();
    QString content = payload.value("content").toString();

    QTcpSocket* receiverSocket = DatabaseManager::instance().getUserSocket(receiver);

    if (receiverSocket) {
        QJsonObject message;
        message["type"] = "TEXT_MESSAGE";
        message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

        QJsonObject messagePayload;
        messagePayload["sender"] = sender;
        messagePayload["receiver"] = receiver;
        messagePayload["content"] = content;

        message["payload"] = messagePayload;

        QJsonDocument doc(message);
        QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
        receiverSocket->write(data);
    } else {
        qDebug() << "Receiver is offline. Message not delivered.";
        // Optionally, store the message for later delivery
    }
}

void ClientHandler::handleFriendRequest(const QJsonObject& payload) {
    QString sender = payload.value("sender").toString();
    QString receiver = payload.value("receiver").toString();

    bool success = DatabaseManager::instance().sendFriendRequest(sender, receiver);

    QJsonObject response;
    response["type"] = "FRIEND_REQUEST_RESPONSE";
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject responsePayload;
    responsePayload["success"] = success;
    responsePayload["error"] = success ? "" : "Failed to send friend request";
    response["payload"] = responsePayload;

    sendResponse(response);

    if (success) {
        QTcpSocket* receiverSocket = DatabaseManager::instance().getUserSocket(receiver);
        if (receiverSocket) {
            QJsonObject forwardRequest;
            forwardRequest["type"] = "FRIEND_REQUEST";
            forwardRequest["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

            QJsonObject forwardPayload;
            forwardPayload["sender"] = sender;
            forwardPayload["receiver"] = receiver;
            forwardRequest["payload"] = forwardPayload;

            QJsonDocument doc(forwardRequest);
            QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
            receiverSocket->write(data);
            receiverSocket->flush();
        } else {
            qDebug() << "Receiver is offline. Friend request not forwarded.";
        }
    }

}

void ClientHandler::handleFetchPendingInvitations(const QJsonObject &payload) {
    QString username = payload.value("username").toString();
    QStringList pendingInvitations = DatabaseManager::instance().getReceivedFriendRequests(username);

    QJsonObject response;
    response["type"] = "PENDING_INVITATIONS_RESPONSE";
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject responsePayload;
    responsePayload["pending_invitations"] = QJsonArray::fromStringList(pendingInvitations);
    response["payload"] = responsePayload;

    sendResponse(response);
}

void ClientHandler::handleFetchContactList(const QJsonObject& payload) {
    QString username = payload.value("username").toString();
    QStringList contacts = DatabaseManager::instance().getContactList(username);

    QJsonObject response;
    response["type"] = "CONTACT_LIST_RESPONSE";
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject responsePayload;
    responsePayload["contacts"] = QJsonArray::fromStringList(contacts);
    response["payload"] = responsePayload;

    sendResponse(response);
}

void ClientHandler::handleFriendRequestResponse(const QJsonObject& payload) {
    QString sender = payload.value("sender").toString();
    QString receiver = payload.value("receiver").toString();
    bool accepted = payload.value("accepted").toBool();

    bool success = DatabaseManager::instance().respondToFriendRequest(sender, receiver, accepted);

    // Send response back to the receiver
    QJsonObject response;
    response["type"] = "FRIEND_REQUEST_RESPONSE_RESULT";
    response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject responsePayload;
    responsePayload["success"] = success;
    responsePayload["sender"] = sender;
    responsePayload["receiver"] = receiver;
    responsePayload["accepted"] = accepted;
    response["payload"] = responsePayload;

    sendResponse(response);

    if (accepted) {
        // Notify both users to refresh contacts
        notifyContactListUpdate(sender);
        notifyContactListUpdate(receiver);
    }
}

void ClientHandler::notifyContactListUpdate(const QString& username) {
    QTcpSocket* userSocket = DatabaseManager::instance().getUserSocket(username);
    if (userSocket) {
        QJsonObject message;
        message["type"] = "CONTACT_LIST_UPDATE";
        message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        message["payload"] = QJsonObject();

        sendResponseToSocket(userSocket, message);
    }
}

void ClientHandler::sendResponseToSocket(QTcpSocket* socket, const QJsonObject& response) {
    QJsonDocument doc(response);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    socket->write(data);
    socket->flush();
}

void ClientHandler::sendResponse(const QJsonObject& response) {
    QJsonDocument doc(response);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    clientSocket->write(data);
    clientSocket->flush();
    qDebug() << "ClientHandler: Response sent to client:" << response;
}