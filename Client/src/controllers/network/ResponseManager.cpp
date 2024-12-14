// src/controllers/network/ResponseManager.cpp
#include "ResponseManager.h"
#include "../../helper/MessageSerializer.h"
#include "NetworkController.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ResponseManager* ResponseManager::m_instance = nullptr;

ResponseManager* ResponseManager::instance() {
    if (!m_instance) {
        m_instance = new ResponseManager();
    }
    return m_instance;
}

ResponseManager::ResponseManager(QObject *parent) : QObject(parent) {
    connect(NetworkController::instance(), &NetworkController::dataReceived,
            this, &ResponseManager::onDataReceived);
}

void ResponseManager::onDataReceived(const QByteArray &data) {
    QString errorString;
    QJsonObject message = MessageSerializer::deserialize(data, errorString);

    if (!errorString.isEmpty()) {
        qWarning() << "JSON parse error:" << errorString;
        return;
    }

    QString type = message.value("type").toString();
    QJsonObject payload = message.value("payload").toObject();

    if (type == "LOGIN_RESPONSE") {
        bool success = payload.value("success").toBool();
        QString error = payload.value("error").toString();
        emit loginResponseReceived(success, error);
    } else if (type == "REGISTER_RESPONSE") {
        bool success = payload.value("success").toBool();
        QString error = payload.value("error").toString();
        emit registerResponseReceived(success, error);
    }
    // Handle new message types for ManageContactsController
    else if (type == "FRIEND_REQUEST_RESPONSE") {
        bool success = payload.value("success").toBool();
        QString messageText = payload.value("message").toString();
        emit friendRequestResponseReceived(success, messageText);
    }
    else if (type == "CANCEL_FRIEND_REQUEST_RESPONSE") {
        bool success = payload.value("success").toBool();
        QString messageText = payload.value("message").toString();
        emit cancelFriendRequestResponseReceived(success, messageText);
    }
    else if (type == "ACCEPT_FRIEND_REQUEST_RESPONSE") {
        bool success = payload.value("success").toBool();
        QString messageText = payload.value("message").toString();
        emit acceptFriendRequestResponseReceived(success, messageText);
    }
    else if (type == "REJECT_FRIEND_REQUEST_RESPONSE") {
        bool success = payload.value("success").toBool();
        QString messageText = payload.value("message").toString();
        emit rejectFriendRequestResponseReceived(success, messageText);
    }
    else if (type == "DELETE_FRIEND_RESPONSE") {
        bool success = payload.value("success").toBool();
        QString messageText = payload.value("message").toString();
        emit deleteFriendResponseReceived(success, messageText);
    }
    else if (type == "GET_FRIENDS_RESPONSE") {
        QJsonArray friendsArray = payload.value("friends").toArray();
        QStringList friendsList;
        for (const QJsonValue &value : friendsArray) {
            friendsList.append(value.toString());
        }
        emit friendsListReceived(friendsList);
    }
    else {
        qWarning() << "Unknown message type:" << type;
    }
}