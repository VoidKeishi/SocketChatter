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
    void handleDeleteGroup(const QJsonObject& request);
    void handleGroupInvite(const QJsonObject& request);
    void handleGroupInviteResponse(const QJsonObject& request);
    void handleFetchGroup(const QJsonObject& request);
};