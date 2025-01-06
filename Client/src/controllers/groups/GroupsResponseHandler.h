#pragma once

#include <QObject>
#include <QJsonObject>
#include "../../viewmodels/GroupViewModel.h"

class GroupsResponseHandler : public QObject {
    Q_OBJECT

public:
    explicit GroupsResponseHandler(GroupViewModel* viewModel, QObject* parent = nullptr);

public slots:
    void handleCreateGroupResponse(const QJsonObject& response);
    void handleFetchGroupsResponse(const QJsonObject& response);
    void handleDeleteGroupResponse(const QJsonObject& response);
    void handleInviteToGroupResponse(const QJsonObject& response);
    void handleGroupInviteResponseReceived(const QJsonObject& response);

private:
    GroupViewModel* m_viewModel;
};