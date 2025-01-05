#include <QJsonArray>

#include "ContactHandler.h"
#include "../core/ConnectionManager.h"
#include "../utils/Logger.h"

ContactHandler::ContactHandler(DatabaseManager* db)
    : BaseHandler(nullptr), contactRepo(db) 
{
    handlers = {
        {"FETCH_FRIEND_LIST", [this](const QJsonObject& p) { handleGetFriendList(p); }},
        {"FETCH_SENT_REQUESTS", [this](const QJsonObject& p) { handleGetSentRequestList(p); }},
        {"FETCH_RECEIVED_REQUESTS", [this](const QJsonObject& p) { handleGetPendingRequests(p); }},
        {"SEND_FRIEND_REQUEST", [this](const QJsonObject& p) { handleSendRequest(p); }},
        {"CANCEL_FRIEND_REQUEST", [this](const QJsonObject& p) { handleCancelSentRequest(p); }},
        {"RESPOND_TO_FRIEND_REQUEST", [this](const QJsonObject& p) { handleResponseRequest(p); }},
        {"DELETE_FRIEND_REQUEST", [this](const QJsonObject& p) { handleDeleteFriend(p); }},
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
        Logger::error("Failed to send friend request");
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
    for (const auto& req : requests) {
        requestArray.append(req);
    }
    
    emit responseReady({
        {"type", "FETCH_RECEIVED_REQUESTS_RESPONSE"},
        {"timestamp", static_cast<int>(QDateTime::currentSecsSinceEpoch())},
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
    
    if (contactRepo.updateRequestStatus(to, from, status)) {
        if (accept) {
            contactRepo.addFriendship(to, from);
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

void ContactHandler::handleDeleteFriend(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString from = payload["from"].toString();
    QString to = payload["to"].toString();
    
    if (contactRepo.areFriends(from, to)) {
        if (contactRepo.deleteFriendship(from, to)) {
            emit responseReady({
                {"type", "FRIEND_DELETED_RESPONSE"},
                {"payload", QJsonObject{
                    {"success", true},
                    {"message", "Friend deleted"},
                    {"from", from},
                    {"to", to}
                }}
            });

            // Check if the 'to' user is connected
            ClientHandler* toClient = ConnectionManager::instance()->getClientHandler(to);
            if (toClient) {
                QJsonObject notification = {
                    {"type", "FRIEND_DELETED_NOTIFICATION"},
                    {"timestamp", static_cast<int>(QDateTime::currentSecsSinceEpoch())},
                    {"payload", QJsonObject{
                        {"from", from},
                        {"message", QString("%1 has removed you from their friends list.").arg(from)}
                    }}
                };
                toClient->sendResponse(notification);
                Logger::info(QString("Sent FRIEND_DELETED_NOTIFICATION to %1").arg(to));
            } else {
                Logger::info(QString("User %1 is not connected. Notification not sent.").arg(to));
            }
        } else {
            emit responseReady({
                {"type", "FRIEND_DELETED_RESPONSE"},
                {"payload", QJsonObject{
                    {"success", false},
                    {"message", "Failed to delete friend"},
                    {"from", from},
                    {"to", to}
                }}
            });
        }
    } else {
        emit responseReady({
            {"type", "FRIEND_DELETED_RESPONSE"},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Not friends with user"},
                {"from", from},
                {"to", to}
            }}
        });
    }
}