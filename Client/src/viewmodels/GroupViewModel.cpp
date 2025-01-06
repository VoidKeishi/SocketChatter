#include "GroupViewModel.h"
#include "../controllers/utils/Logger.h"
#include "../controllers/session/UserManager.h"

GroupViewModel::GroupViewModel(QObject* parent)
    : QObject(parent)
{
}

// =================== UI Actions ===================
void GroupViewModel::sendGroupCreateRequest(const QString& groupName) {
    if (groupName.isEmpty()) {
        Logger::error("Group name cannot be empty");
        emit groupActionError("Group name cannot be empty");
        return;
    }
    if (m_groups.contains(groupName)) {
        Logger::error("Group already exists");
        emit groupActionError("Group already exists");
        return;
    }
    
    Logger::debug("Sending group create request: " + groupName);
    emit groupActionRequested(GroupAction::CreateGroup, 
                            UserManager::instance()->currentUser(), 
                            groupName);
}

void GroupViewModel::sendGroupInvitation(const QString& groupName, const QString& friendName) {
    if (groupName.isEmpty() || friendName.isEmpty()) {
        Logger::error("Group name and friend name must not be empty");
        emit groupActionError("Group name and friend name must not be empty");
        return;
    }
    if (!m_groups.contains(groupName)) {
        Logger::error("Group does not exist");
        emit groupActionError("Group does not exist");
        return;
    }
    
    Logger::debug(QString("Inviting %1 to group %2").arg(friendName, groupName));
    emit groupActionRequested(GroupAction::InviteGroupMember,
                            UserManager::instance()->currentUser(),
                            groupName,
                            friendName);
}

void GroupViewModel::respondGroupInvitation(const QString& groupName, bool accept) {
    if (groupName.isEmpty()) {
        Logger::error("Group name cannot be empty");
        return;
    }

    if (accept) {
        Logger::debug("Accepting invitation to group: " + groupName);
        addGroup(groupName);
    } else {
        Logger::debug("Rejecting invitation to group: " + groupName);
    }
    removeGroupInvitation(groupName);
}

void GroupViewModel::fetchGroups() {
    Logger::debug("Fetching groups");
    emit groupActionRequested(GroupAction::FetchGroups,
                            UserManager::instance()->currentUser(), 
                            "");
}

void GroupViewModel::fetchGroupInvitations() {
    Logger::debug("Fetching group invitations");
    // Similar structure to fetchGroups
    // Will be handled by the server response
}

// =================== Update ===================
void GroupViewModel::addGroupInvitation(const QString& group) {
    Logger::debug("Adding group invitation: " + group);
    if (!m_receivedInvitations.contains(group)) {
        m_receivedInvitations.append(group);
        emit receivedInvitationsChanged();
    }
}

void GroupViewModel::removeGroupInvitation(const QString& group) {
    if (m_receivedInvitations.removeOne(group)) {
        emit receivedInvitationsChanged();
    }
}

void GroupViewModel::addGroup(const QString& group) {
    if (!m_groups.contains(group)) {
        m_groups.append(group);
        emit groupsChanged();
        emit groupActionSuccess("Successfully joined group: " + group);
    }
}

void GroupViewModel::removeGroup(const QString& group) {
    if (m_groups.removeOne(group)) {
        emit groupsChanged();
        emit groupActionSuccess("Successfully left group: " + group);
    }
}