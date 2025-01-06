#pragma once

#include <QObject>
#include <QJsonObject>
#include "../../viewmodels/GroupViewModel.h"

class GroupsNotificationHandler : public QObject {
    Q_OBJECT

public:
    explicit GroupsNotificationHandler(GroupViewModel* viewModel, QObject* parent = nullptr);

public slots:
    void handleGroupUpdatedNotification(const QJsonObject& notification);
    void handleGroupMemberAddedNotification(const QJsonObject& notification);
    void handleGroupDeletedNotification(const QJsonObject& notification);
    void handleGroupInvitationReceivedNotification(const QJsonObject& notification);

private:
    GroupViewModel* m_viewModel;
};