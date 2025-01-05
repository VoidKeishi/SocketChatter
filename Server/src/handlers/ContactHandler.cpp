#include <QJsonArray>

#include "ContactHandler.h"
#include "../core/ConnectionManager.h"
#include "../utils/Logger.h"
#include "../utils/ResponseFactory.h"

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
        emit responseReady(ResponseFactory::createFriendRequestResponse(
            true, 
            "Friend request sent", 
            to
        ));
    } else {
        qDebug() << "Failed to send friend request";
        emit responseReady(ResponseFactory::createFriendRequestResponse(
            false, 
            "Failed to send friend request", 
            to
        ));
    }
}

void ContactHandler::handleCancelSentRequest(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString from = payload["from"].toString();
    QString to = payload["to"].toString();
    
    if (contactRepo.updateRequestStatus(from, to, "cancelled")) {
        emit responseReady(ResponseFactory::cancelFriendRequestResponse(
            true, 
            "Friend request cancelled", 
            to
        ));
    } else {
        emit responseReady(ResponseFactory::cancelFriendRequestResponse(
            false, 
            "Failed to cancel friend request", 
            to
        ));
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
    
    emit responseReady(ResponseFactory::getFriendListResponse(true, friendArray));
}

void ContactHandler::handleGetSentRequestList(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    QVector<QString> sent = contactRepo.getSentRequestList(username);
    
    QJsonArray sentArray;
    for (const QString& sentName : sent) {
        sentArray.append(sentName);
    }
    emit responseReady(ResponseFactory::getSendRequestListResponse(true, sentArray));
}

void ContactHandler::handleGetPendingRequests(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    auto requests = contactRepo.getPendingRequests(username);
    
    QJsonArray requestArray;
    for (const auto& req : requests) {
        requestArray.append(req);
    }
    
    emit responseReady(ResponseFactory::getPendingRequestsResponse(true, requestArray));
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
        
        emit responseReady(ResponseFactory::createFriendRequestResponse(
            true, 
            QString("Friend request %1").arg(status), 
            from, 
            to
        ));
    } else {
        emit responseReady(ResponseFactory::createFriendRequestResponse(
            false, 
            "Failed to process friend request", 
            from, 
            to
        ));
    }
}

void ContactHandler::handleDeleteFriend(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString from = payload["from"].toString();
    QString to = payload["to"].toString();
    
    if (contactRepo.areFriends(from, to)) {
        if (contactRepo.deleteFriendship(from, to)) {
            emit responseReady(ResponseFactory::deleteFriendResponse(
                true, 
                "Friend deleted", 
                from, 
                to
            ));
            // emit responseReady({
            //     {"type", "FRIEND_DELETED_RESPONSE"},
            //     {"payload", QJsonObject{
            //         {"success", true},
            //         {"message", "Friend deleted"},
            //         {"from", from},
            //         {"to", to}
            //     }}
            // });

            // Check if the 'to' user is connected
            ClientHandler* toClient = ConnectionManager::instance()->getClientHandler(to);
            if (toClient) {
                toClient->sendResponse(ResponseFactory::announceFriendDeletion(
                    from, 
                    QString("%1 has removed you from their friends list.").arg(from)
                ));
                Logger::info(QString("Sent FRIEND_DELETED_NOTIFICATION to %1").arg(to));
            } else {
                Logger::info(QString("User %1 is not connected. Notification not sent.").arg(to));
            }
        } else {
            emit responseReady(ResponseFactory::deleteFriendResponse(
                false, 
                "Failed to delete friend", 
                from, 
                to
            ));
            // emit responseReady({
            //     {"type", "FRIEND_DELETED_RESPONSE"},
            //     {"payload", QJsonObject{
            //         {"success", false},
            //         {"message", "Failed to delete friend"},
            //         {"from", from},
            //         {"to", to}
            //     }}
            // });
        }
    } else {
        emit responseReady(ResponseFactory::deleteFriendResponse(
            false, 
            "Not friends with user", 
            from, 
            to
        ));
        // emit responseReady({
        //     {"type", "FRIEND_DELETED_RESPONSE"},
        //     {"payload", QJsonObject{
        //         {"success", false},
        //         {"message", "Not friends with user"},
        //         {"from", from},
        //         {"to", to}
        //     }}
        // });
    }
}