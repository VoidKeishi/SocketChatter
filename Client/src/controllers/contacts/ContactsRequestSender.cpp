#include "ContactsRequestSender.h"
#include "../utils/RequestFactory.h"
#include "../network/NetworkController.h"

ContactsRequestSender::ContactsRequestSender(QObject* parent) 
    : QObject(parent)
{
    connect(this, &ContactsRequestSender::sendRequest,
            NetworkController::instance(), &NetworkController::sendData);
}

void ContactsRequestSender::requestAddFriend(const QString& from, const QString& to) {
    emit sendRequest(RequestFactory::createSendFriendRequest(from, to));
}

void ContactsRequestSender::requestCancelFriend(const QString& from, const QString& to) {
    emit sendRequest(RequestFactory::createCancelFriendRequest(from, to));
}

void ContactsRequestSender::requestRespondToFriend(const QString& from, const QString& to, bool accept) {
    emit sendRequest(RequestFactory::createResponseFriendRequest(from, to, accept));
}

void ContactsRequestSender::requestDeleteFriend(const QString& from, const QString& to) {
    emit sendRequest(RequestFactory::createDeleteFriendRequest(from, to));
}

void ContactsRequestSender::requestFetchFriends(const QString& username) {
    emit sendRequest(RequestFactory::createFetchFriendsListRequest(username));
}

void ContactsRequestSender::requestFetchSentRequests(const QString& username) {
    emit sendRequest(RequestFactory::createFetchSentRequestsRequest(username));
}

void ContactsRequestSender::requestFetchReceivedRequests(const QString& username) {
    emit sendRequest(RequestFactory::createFetchReceivedRequestsRequest(username));
}