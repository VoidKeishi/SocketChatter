#pragma once
#include "BaseHandler.h"
#include "../repositories/GroupRepository.h"

class GroupHandler : public BaseHandler {
    Q_OBJECT
public:
    explicit GroupHandler(DatabaseManager* db);

private:
    GroupRepository groupRepo;

    void handleCreateGroup(const QJsonObject& request);
    void handleInviteMember(const QJsonObject& request);
    void handleAcceptInvitation(const QJsonObject& request);
    void handleRejectInvitation(const QJsonObject& request); 
    void handleLeaveGroup(const QJsonObject& request);
};