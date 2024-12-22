#pragma once
#include "BaseHandler.h"
#include "../repositories/ContactRepository.h"

class ContactHandler : public BaseHandler {
    Q_OBJECT
public:
    explicit ContactHandler(DatabaseManager* db);

private:
    ContactRepository contactRepo;
    void handleGetFriendList(const QJsonObject& payload);
    void handleSendRequest(const QJsonObject& payload);
    void handleCancelSentRequest(const QJsonObject& payload);
    void handleResponseRequest(const QJsonObject& payload);
    void handleGetPendingRequests(const QJsonObject& payload);
};