#include "ContactHandler.h"
#include <QDebug>
#include <QJsonArray>

ContactHandler::ContactHandler(DatabaseManager* db)
    : BaseHandler(nullptr), contactRepo(db) 
{
    handlers = {
        {"FETCH_FRIEND_LIST", [this](const QJsonObject& p) { handleGetFriendList(p); }},
        {"FETCH_SENT_REQUESTS", [this](const QJsonObject& p) { handleGetSentRequestList(p); }},
        {"FETCH_RECEIVED_REQUESTS", [this](const QJsonObject& p) { handleGetPendingRequests(p); }},
        {"SEND_FRIEND_REQUEST", [this](const QJsonObject& p) { handleSendRequest(p); }},
        {"CANCEL_FRIEND_REQUEST", [this](const QJsonObject& p) { handleCancelSentRequest(p); }},
        {"FRIEND_REQUEST_RESPONSE", [this](const QJsonObject& p) { handleResponseRequest(p); }},
        {"FRIEND_REQUEST_CANCEL", [this](const QJsonObject& p) { handleCancelSentRequest(p); }},
        {"FRIEND_REQUESTS_GET", [this](const QJsonObject& p) { handleGetPendingRequests(p); }},
        {"RESPOND_TO_FRIEND_REQUEST", [this](const QJsonObject& p) { handleResponseRequest(p); }},
    };
}

void ContactHandler::handleSendRequest(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString from = payload["from"].toString();
    QString to = payload["to"].toString();

    if (contactRepo.createFriendRequest(from, to)) {
        emit responseReady({
            {"type", "FRIEND_REQUEST_SENT_RESPONSE"},
            {"payload", QJsonObject{
                {"success", true},
                {"message", "Friend request sent"},
                {"to", to}
            }}
        });
    } else {
        qDebug () << "Failed to send friend request";
        emit responseReady({
            {"type", "FRIEND_REQUEST_SENT_RESPONSE"},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Failed to send friend request"},
                {"to", to}
            }}
        });
    }
}

void ContactHandler::handleCancelSentRequest(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
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
                {"message", "Failed to cancel friend request"},
                {"to", to}
            }}
        });
    }
}

void ContactHandler::handleGetFriendList(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    QVector<QString> friends = contactRepo.getFriendList(username);
    
    QJsonArray friendArray;
    for (const QString& friendName : friends) {
        friendArray.append(friendName);
    }
    
    emit responseReady({
        {"type", "FETCH_FRIEND_LIST_RESPONSE"},
        {"payload", QJsonObject{
            {"success", true},
            {"friends", friendArray}
        }}
    });
}

void ContactHandler::handleGetSentRequestList(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    QVector<QString> sent = contactRepo.getSentRequestList(username);
    
    QJsonArray sentArray;
    for (const QString& sentName : sent) {
        sentArray.append(sentName);
    }
    emit responseReady({
        {"type", "FETCH_SENT_REQUESTS_RESPONSE"},
        {"payload", QJsonObject{
            {"success", true},
            {"sent", sentArray}
        }}
    });
}

void ContactHandler::handleGetPendingRequests(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    auto requests = contactRepo.getPendingRequests(username);
    
    QJsonArray requestArray;
    for (const auto& request : requests) {
        requestArray.append(QJsonObject{
            {"from", request},
        });
    }
    
    emit responseReady({
        {"type", "FETCH_RECEIVED_REQUESTS_RESPONSE"},
        {"payload", QJsonObject{
            {"success", true},
            {"requests", requestArray}
        }}
    });
}

void ContactHandler::handleResponseRequest(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString from = payload["from"].toString();
    QString to = payload["to"].toString();
    bool accept = payload["accept"].toBool();

    QString status = accept ? "accepted" : "rejected";
    
    if (contactRepo.updateRequestStatus(from, to, status)) {
        if (accept) {
            // Add both users as friends if accepted
            contactRepo.addFriendship(from, to);
        }
        
        emit responseReady({
            {"type", "FRIEND_REQUEST_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", true},
                {"message", QString("Friend request %1").arg(status)},
                {"from", from},
                {"to", to}
            }}
        });
    } else {
        emit responseReady({
            {"type", "FRIEND_REQUEST_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Failed to process friend request"},
                {"from", from},
                {"to", to}
            }}
        });
    }
}