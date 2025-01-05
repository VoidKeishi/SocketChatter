#include <QJsonArray>

#include "ContactsNotificationHandler.h"
#include "../session/UserManager.h"
#include "../utils/Logger.h"


ContactsNotificationHandler::ContactsNotificationHandler(ContactViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
}

void ContactsNotificationHandler::handleFriendRequestNotification(const QJsonObject& notification){
    QString to = notification.value("to").toString();
    if (to != UserManager::instance()->currentUser()) {
        Logger::error("Received friend request for another user: " + to);
        return;
    }
    QString from = notification.value("from").toString();
    m_viewModel->addReceivedRequest(from);
    Logger::debug("Received friend request from: " + notification.value("from").toString());
}

void ContactsNotificationHandler::handleFriendRequestCanceledNotification(const QJsonObject& notification){
    QString to = notification.value("to").toString();
    if (to != UserManager::instance()->currentUser()) {
        Logger::error("Received cancel request for another user: " + to);
        return;
    }
    QString from = notification.value("from").toString();
    m_viewModel->removeReceivedRequest(from);
    Logger::debug("Friend request from " + notification.value("from").toString() + " was cancelled");
}

void ContactsNotificationHandler::handleFriendRequestResponseNotification(const QJsonObject& notification){
    QString to = notification.value("to").toString();
    if (to != UserManager::instance()->currentUser()) {
        Logger::error("Received response for another user: " + to);
        return;
    }
    QString from = notification.value("from").toString();
    bool accept = notification.value("accept").toBool();
    if (accept) {
        m_viewModel->addFriend(from);
    }
    m_viewModel->removeSentRequest(from);
    Logger::debug("Friend request from " + notification.value("from").toString() + " was " + (accept ? "accepted" : "rejected"));
}

void ContactsNotificationHandler::handleFriendDeletedNotification(const QJsonObject& notification){
    QString to = notification.value("to").toString();
    if (to != UserManager::instance()->currentUser()) {
        Logger::error("Received delete request for another user: " + to);
        return;
    }
    QString from = notification.value("from").toString();
    m_viewModel->removeFriend(from);
    Logger::debug("Friend " + notification.value("from").toString() + " was deleted");
}
