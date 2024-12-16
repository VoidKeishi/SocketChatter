#include "RequestManager.h"
#include "NetworkController.h"
#include "../../helper/MessageSerializer.h"
#include <QCryptographicHash>

RequestManager* RequestManager::m_instance = nullptr;

RequestManager* RequestManager::instance() {
    if (!m_instance) {
        m_instance = new RequestManager();
    }
    return m_instance;
}

RequestManager::RequestManager(QObject *parent) : QObject(parent) {}

void RequestManager::sendLoginRequest(const QString &username, const QString &password) {
    QJsonObject payload;
    payload["username"] = username;
    payload["password"] = hashPassword(password);
    sendRequest("LOGIN_REQUEST", payload);
}

void RequestManager::sendRegisterRequest(const QString &username, const QString &password) {
    QJsonObject payload;
    payload["username"] = username;
    payload["password"] = hashPassword(password);
    sendRequest("REGISTER_REQUEST", payload);
}

QString RequestManager::hashPassword(const QString &password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

void RequestManager::sendRequest(const QString &type, const QJsonObject &payload) {
    QJsonObject request;
    request["type"] = type;
    request["payload"] = payload;
    NetworkController::instance()->sendData(MessageSerializer::serialize(request));
}

void RequestManager::sendFriendRequest(const QString &username) {
    QJsonObject payload;
    payload["username"] = username;

    QJsonObject request;
    request["type"] = "FRIEND_REQUEST_RESPONSE"; // Ensure this matches server's expected type
    request["payload"] = payload;

    NetworkController::instance()->sendData(MessageSerializer::serialize(request));
}

void RequestManager::cancelFriendRequest(const QString &username) {
    QJsonObject payload;
    payload["username"] = username;

    QJsonObject request;
    request["type"] = "CANCEL_FRIEND_REQUEST_RESPONSE"; // Ensure this matches server's expected type
    request["payload"] = payload;

    NetworkController::instance()->sendData(MessageSerializer::serialize(request));
}

void RequestManager::acceptFriendRequest(const QString &username) {
    QJsonObject payload;
    payload["username"] = username;

    QJsonObject request;
    request["type"] = "ACCEPT_FRIEND_REQUEST_RESPONSE"; // Ensure this matches server's expected type
    request["payload"] = payload;

    NetworkController::instance()->sendData(MessageSerializer::serialize(request));
}

void RequestManager::rejectFriendRequest(const QString &username) {
    QJsonObject payload;
    payload["username"] = username;

    QJsonObject request;
    request["type"] = "REJECT_FRIEND_REQUEST_RESPONSE"; // Ensure this matches server's expected type
    request["payload"] = payload;

    NetworkController::instance()->sendData(MessageSerializer::serialize(request));
}

void RequestManager::deleteFriend(const QString &username) {
    QJsonObject payload;
    payload["username"] = username;

    QJsonObject request;
    request["type"] = "DELETE_FRIEND_RESPONSE"; // Ensure this matches server's expected type
    request["payload"] = payload;

    NetworkController::instance()->sendData(MessageSerializer::serialize(request));
}

void RequestManager::getFriendsList() {
    QJsonObject request;
    request["type"] = "GET_FRIENDS_RESPONSE"; // Ensure this matches server's expected type
    request["payload"] = QJsonObject();

    NetworkController::instance()->sendData(MessageSerializer::serialize(request));
}