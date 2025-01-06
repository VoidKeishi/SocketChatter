#pragma once
#include "BaseHandler.h"
#include "../repositories/ContactRepository.h"

class ContactHandler : public BaseHandler {
    Q_OBJECT

public:
    explicit ContactHandler(DatabaseManager* db);
private:
    ContactRepository contactRepo;
    void handleSendRequest(const QJsonObject& request);
    void handleCancelSentRequest(const QJsonObject& request);
    void handleResponseRequest(const QJsonObject& request);
    void handleDeleteFriend(const QJsonObject& request);
    // Handle fetch data requests
    void handleGetFriendList(const QJsonObject& request);
    void handleGetPendingRequests(const QJsonObject& request);
    void handleGetSentRequestList(const QJsonObject& request);
    void handleGetOnlineStatus(const QJsonObject& request);
};