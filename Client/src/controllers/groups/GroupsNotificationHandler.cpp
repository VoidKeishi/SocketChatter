#include "GroupsNotificationHandler.h"
#include "../session/UserManager.h"
#include "../utils/Logger.h"

GroupsNotificationHandler::GroupsNotificationHandler(GroupViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
}

void GroupsNotificationHandler::handleGroupUpdatedNotification(const QJsonObject& notification) {
    QString groupName = notification.value("groupName").toString();
    QString updateInfo = notification.value("updateInfo").toString();
    Logger::info(QString("Group %1 updated: %2").arg(groupName, updateInfo));
}

void GroupsNotificationHandler::handleGroupMemberAddedNotification(const QJsonObject& notification) {
    QString groupName = notification.value("groupName").toString();
    QString memberName = notification.value("memberName").toString();
    Logger::info(QString("New member %1 added to group %2").arg(memberName, groupName));
}

void GroupsNotificationHandler::handleGroupDeletedNotification(const QJsonObject& notification) {
    QString groupName = notification.value("groupName").toString();
    m_viewModel->removeGroup(groupName);
    Logger::info("Group deleted: " + groupName);
}

void GroupsNotificationHandler::handleGroupInvitationReceivedNotification(const QJsonObject& notification) {
    QString groupName = notification.value("groupName").toString();
    QString inviter = notification.value("from").toString();
    
    if (notification.value("to").toString() != UserManager::instance()->currentUser()) {
        Logger::error("Received group invitation for another user");
        return;
    }

    m_viewModel->addGroupInvitation(groupName);
    Logger::info(QString("Received invitation to group %1 from %2").arg(groupName, inviter));
}