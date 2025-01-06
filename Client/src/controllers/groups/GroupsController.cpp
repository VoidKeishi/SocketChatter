#include "GroupsController.h"
#include "GroupsRequestSender.h"
#include "GroupsResponseHandler.h"
#include "GroupsNotificationHandler.h"

GroupsController::GroupsController(GroupViewModel* viewModel, QObject* parent)
    : QObject(parent) 
    , m_viewModel(viewModel) 
    , m_requestSender(new GroupsRequestSender(this))
    , m_responseHandler(new GroupsResponseHandler(viewModel, this))
    , m_notificationHandler(new GroupsNotificationHandler(viewModel, this))
{
    handlers = {
        // Responses
        { "GROUP_CREATE_RESPONSE", [this](const QJsonObject& payload) { m_responseHandler->handleCreateGroupResponse(payload); }},
        { "GROUP_FETCH_RESPONSE",  [this](const QJsonObject& payload) { m_responseHandler->handleFetchGroupsResponse(payload); }},

        // Notifications
        { "GROUP_INVITE_NOTIFICATION", [this](const QJsonObject& payload) { m_notificationHandler->handleGroupInvitationReceivedNotification(payload); }},
        { "GROUP_UPDATED_NOTIFICATION", [this](const QJsonObject& payload) { m_notificationHandler->handleGroupUpdatedNotification(payload); }},
        { "GROUP_MEMBER_ADDED_NOTIFICATION", [this](const QJsonObject& payload) { m_notificationHandler->handleGroupMemberAddedNotification(payload); }},
    };

    connect(m_viewModel, &GroupViewModel::groupActionRequested, 
            this, &GroupsController::handleGroupAction);
    connect(this, &GroupsController::sendRequest, 
            NetworkController::instance(), &NetworkController::sendData);
}

bool GroupsController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

// Handle responses and notifications
void GroupsController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        handlers[type](payload);
    }
}
// Handle UI actions
void GroupsController::handleGroupAction(GroupAction action, const QString& sender, const QString& receiver, const QString& content) {
    switch (action) {
    case GroupAction::CreateGroup:
        Logger::debug("CreateGroup");
        m_requestSender->requestCreateGroup(sender, receiver, content);
        break;
    case GroupAction::FetchGroups:
        m_requestSender->requestFetchGroups(sender);
        break;
    case GroupAction::InviteGroupMember:
        m_requestSender->requestInviteToGroup(sender, receiver, content);
        break;
    case GroupAction::DeleteGroup:
        m_requestSender->requestDeleteGroup(sender, receiver);
        break;
    default:
        Logger::error("Invalid group action");
        break;
    }
}