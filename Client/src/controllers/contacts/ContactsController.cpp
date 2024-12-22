#include "ContactsController.h"
#include "../utils/RequestFactory.h"
#include "../session/UserManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

// Constructor
ContactsController::ContactsController(QObject *parent)
    : QObject(parent) {
}

// Methods to handle UI actions
void ContactsController::requestAddFriend(const QString &to) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createSendFriendRequest(from, to);
    emit sendAddFriend(requestData);
}

void ContactsController::requestCancelFriend(const QString &username) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createCancelFriendRequest(from, username);
    emit sendCancelFriend(requestData);
}

void ContactsController::requestRespondToFriend(const QString &username, bool accept) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createResponseFriendRequest(from, username, accept);
    emit sendRespondToFriend(requestData);
}

void ContactsController::requestDeleteFriend(const QString &username) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createDeleteFriendRequest(from, username);
    emit sendDeleteFriend(requestData);
}

void ContactsController::requestFetchFriends() {
    QByteArray requestData = RequestFactory::createFetchFriendsListRequest();
    emit sendFetchFriends(requestData);
}

// Slots to handle responses
void ContactsController::handleFriendRequestSent(const QJsonObject &response) {
    
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    emit addFriendResult(success, message);
}

void ContactsController::handleFriendRequestCanceled(const QJsonObject &response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    emit cancelFriendResult(success, message);
}

void ContactsController::handleFriendRequestResponse(const QJsonObject &response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    emit friendResponseResult(success, message);
}

void ContactsController::handleFriendDeleted(const QJsonObject &response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    emit friendDeletedResult(success, message);
}

void ContactsController::handleFriendsListFetched(const QJsonObject &response) {
    QStringList friends;
    QJsonArray friendsArray = response.value("friends").toArray();
    for (const QJsonValue &value : friendsArray) {
        friends << value.toString();
    }
    emit friendsListResult(friends);
}