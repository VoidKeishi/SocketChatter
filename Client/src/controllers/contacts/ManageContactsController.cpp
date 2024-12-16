// src/controllers/ManageContactsController.cpp
#include "ManageContactsController.h"
#include "../network/ResponseManager.h"
#include "../network/RequestManager.h"

ManageContactsController::ManageContactsController(QObject *parent) 
    : QObject(parent) {
    // Connect ResponseManager signals to ManageContactsController signals
    connect(ResponseManager::instance(), &ResponseManager::friendRequestResponseReceived,
            this, [&](bool success, QString message) {
        emit friendRequestSent(success, message);
    });

    connect(ResponseManager::instance(), &ResponseManager::cancelFriendRequestResponseReceived,
            this, [&](bool success, QString message) {
        emit friendRequestCanceled(success, message);
    });

    connect(ResponseManager::instance(), &ResponseManager::acceptFriendRequestResponseReceived,
            this, [&](bool success, QString message) {
        emit friendRequestAccepted(success, message);
    });

    connect(ResponseManager::instance(), &ResponseManager::rejectFriendRequestResponseReceived,
            this, [&](bool success, QString message) {
        emit friendRequestRejected(success, message);
    });

    connect(ResponseManager::instance(), &ResponseManager::deleteFriendResponseReceived,
            this, [&](bool success, QString message) {
        emit friendDeleted(success, message);
    });

    connect(ResponseManager::instance(), &ResponseManager::friendsListReceived,
            this, [&](const QStringList &friends) {
        emit friendsListFetched(friends);
    });
}


void ManageContactsController::sendFriendRequest(const QString &username) {
    RequestManager::instance()->sendFriendRequest(username);
}

void ManageContactsController::cancelFriendRequest(const QString &username) {
    RequestManager::instance()->cancelFriendRequest(username);
}

void ManageContactsController::acceptFriendRequest(const QString &username) {
    RequestManager::instance()->acceptFriendRequest(username);
}

void ManageContactsController::rejectFriendRequest(const QString &username) {
    RequestManager::instance()->rejectFriendRequest(username);
}

void ManageContactsController::deleteFriend(const QString &username) {
    RequestManager::instance()->deleteFriend(username);
}

void ManageContactsController::fetchFriendsList() {
    RequestManager::instance()->getFriendsList();
}