#include "GroupsResponseHandler.h"
#include "../utils/Logger.h"
#include <QJsonArray>

GroupsResponseHandler::GroupsResponseHandler(GroupViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
}

void GroupsResponseHandler::handleCreateGroupResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString groupName = response.value("groupName").toString();
        m_viewModel->addGroup(groupName);
        Logger::info("Group created successfully: " + groupName);
    } else {
        Logger::error("Failed to create group: " + message);
    }
}

void GroupsResponseHandler::handleFetchGroupsResponse(const QJsonObject& response) {
    QJsonArray groups = response.value("groups").toArray();
    for (const QJsonValue& value : groups) {
        m_viewModel->addGroup(value.toString());
    }
    Logger::info("Groups fetched successfully");
}

void GroupsResponseHandler::handleDeleteGroupResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString groupName = response.value("groupName").toString();
        m_viewModel->removeGroup(groupName);
        Logger::info("Group deleted successfully: " + groupName);
    } else {
        Logger::error("Failed to delete group: " + message);
    }
}

void GroupsResponseHandler::handleInviteToGroupResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    Logger::info(success ? "Invitation sent successfully" : "Failed to send invitation: " + message);
}

void GroupsResponseHandler::handleGroupInviteResponseReceived(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    QString groupName = response.value("groupName").toString();
    
    if (success) {
        m_viewModel->removeGroupInvitation(groupName);
        Logger::info("Invitation response handled: " + message);
    } else {
        Logger::error("Failed to handle invitation response: " + message);
    }
}