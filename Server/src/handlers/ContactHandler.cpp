#include "ContactHandler.h"
#include <QDebug>
#include <QJsonArray>

ContactHandler::ContactHandler(DatabaseManager* db)
    : BaseHandler(nullptr), contactRepo(db) 
{
    handlers = {
        {"FRIEND_LIST_GET", [this](const QJsonObject& p) { handleGetFriendList(p); }},
        {"SEND_FRIEND_REQUEST", [this](const QJsonObject& p) { handleSendRequest(p); }},
        {"FRIEND_REQUEST_RESPONSE", [this](const QJsonObject& p) { handleResponseRequest(p); }},
        {"FRIEND_REQUEST_CANCEL", [this](const QJsonObject& p) { handleCancelSentRequest(p); }},
        {"FRIEND_REQUESTS_GET", [this](const QJsonObject& p) { handleGetPendingRequests(p); }}
    };
}

void ContactHandler::handleSendRequest(const QJsonObject& payload) {
    QString from = payload["from"].toString();
    QString to = payload["to"].toString();

    if (contactRepo.createFriendRequest(from, to)) {
        emit responseReady({
            {"type", "FRIEND_REQUEST_RESPONSE"},
            {"payload", QJsonObject{
                {"success", true},
                {"message", "Friend request sent"},
                {"to", to}
            }}
        });
    } else {
        emit responseReady({
            {"type", "FRIEND_REQUEST_RESPONSE"},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Failed to send friend request"}
            }}
        });
    }
}

void ContactHandler::handleGetFriendList(const QJsonObject& payload) {
    QString username = payload["username"].toString();
    QVector<QString> friends = contactRepo.getFriendList(username);
    
    QJsonArray friendArray;
    for (const QString& friendName : friends) {
        friendArray.append(friendName);
    }
    
    emit responseReady({
        {"type", "FRIEND_LIST_RESPONSE"},
        {"payload", QJsonObject{
            {"success", true},
            {"friends", friendArray}
        }}
    });
}

void ContactHandler::handleGetPendingRequests(const QJsonObject& payload) {
    QString username = payload["username"].toString();
    auto requests = contactRepo.getPendingRequests(username);
    
    QJsonArray requestArray;
    for (const auto& request : requests) {
        requestArray.append(QJsonObject{
            {"from", request.first},
            {"to", request.second}
        });
    }
    
    emit responseReady({
        {"type", "FRIEND_REQUESTS_RESPONSE"},
        {"payload", QJsonObject{
            {"success", true},
            {"requests", requestArray}
        }}
    });
}

void ContactHandler::handleResponseRequest(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString sender = payload["sender"].toString();
    QString receiver = payload["receiver"].toString();
    QString action = payload["action"].toString();
    QString status = (action == "accept") ? "accepted" : "rejected";
    int code = (action == "accept") ? 0 : 3004;
    
    if (contactRepo.updateRequestStatus(sender, receiver, status)) {
        emit responseReady({
            {"type", "FRIEND_REQUEST_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"receiver", receiver},
                {"code", code}
            }}
        });
    } else {
        emit responseReady({
            {"type", "FRIEND_REQUEST_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"receiver", receiver},
                {"code", 3005}, // Internal error code
                {"message", QString("Failed to %1 friend request").arg(action)}
            }}
        });
    }
}

void ContactHandler::handleCancelSentRequest(const QJsonObject& payload) {
    QString from = payload["from"].toString();
    QString to = payload["to"].toString();
    
    if (contactRepo.updateRequestStatus(from, to, "cancelled")) {
        emit responseReady({
            {"type", "FRIEND_REQUEST_CANCEL_RESPONSE"},
            {"payload", QJsonObject{
                {"success", true},
                {"message", "Friend request cancelled"},
                {"to", to}
            }}
        });
    } else {
        emit responseReady({
            {"type", "FRIEND_REQUEST_CANCEL_RESPONSE"},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Failed to cancel friend request"}
            }}
        });
    }
}