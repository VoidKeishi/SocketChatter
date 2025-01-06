#include "GroupsRequestSender.h"
#include "../network/NetworkController.h"
#include "../utils/RequestFactory.h"

GroupsRequestSender::GroupsRequestSender(QObject* parent) 
    : QObject(parent)
{
    connect(this, &GroupsRequestSender::sendRequest,
            NetworkController::instance(), &NetworkController::sendData);
}

void GroupsRequestSender::requestCreateGroup(const QString& sender, const QString& groupName, const QString& groupDetails) {
    emit sendRequest(RequestFactory::createGroupCreateRequest(sender, groupName, groupDetails));
}

void GroupsRequestSender::requestFetchGroups(const QString& sender) {
    emit sendRequest(RequestFactory::createGroupFetchRequest(sender));
}

void GroupsRequestSender::requestDeleteGroup(const QString& sender, const QString& groupName) {
    emit sendRequest(RequestFactory::createGroupDeleteRequest(sender, groupName));
}

void GroupsRequestSender::requestInviteToGroup(const QString& sender, const QString& groupName, const QString& invitee) {
    emit sendRequest(RequestFactory::createGroupInviteRequest(sender, groupName, invitee));
}

void GroupsRequestSender::requestRespondToInvitation(const QString& sender, const QString& groupName, bool accept) {
    emit sendRequest(RequestFactory::createGroupInviteResponseRequest(sender, groupName, accept));
}
