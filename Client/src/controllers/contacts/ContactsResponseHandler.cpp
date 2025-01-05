#include <QJsonArray>

#include "ContactsResponseHandler.h"
#include "../utils/Logger.h"


ContactsResponseHandler::ContactsResponseHandler(ContactViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
}

// Slots to handle fetch response
void ContactsResponseHandler::handleFriendsListFetched(const QJsonObject& response) {
    QStringList friends;
    QJsonArray friendsArray = response.value("friends").toArray();
    for (const QJsonValue& value : friendsArray) {
        friends << value.toString();
    }
    m_viewModel->setFriends(friends);
}

void ContactsResponseHandler::handleSentListFetched(const QJsonObject& response) {
    QStringList sentRequests;
    QJsonArray sentArray = response.value("sent").toArray();
    for (const QJsonValue& value : sentArray) {
        sentRequests << value.toString();
    }
    m_viewModel->setSentRequests(sentRequests);
    for (const auto& request : sentRequests) {
        Logger::debug("Request: " + request);
    }
}

void ContactsResponseHandler::handleReceivedListFetched(const QJsonObject& response) {
    QStringList receivedRequests;
    QJsonArray receivedArray = response.value("requests").toArray();
    for (const QJsonValue& value : receivedArray) {
        receivedRequests << value.toString(); 
    }
    m_viewModel->setReceivedRequests(receivedRequests);
}

// Slots to handle update responses
void ContactsResponseHandler::handleFriendRequestSent(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("to").toString();
        m_viewModel->addSentRequest(username);
        Logger::debug("Friend request sent success: " + username);
    } else {
        Logger::error("Error: " + message);
    }
}

void ContactsResponseHandler::handleFriendRequestCanceled(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("to").toString();
        m_viewModel->removeSentRequest(username);
        Logger::debug("Friend request canceled: " + username);
    } else {
        Logger::error("Error: " + message);
    }
}

void ContactsResponseHandler::handleFriendRequestResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    bool accept = response.value("accept").toBool();
    QString message = response.value("message").toString();
    QString username = response.value("to").toString();
    
    if (success) {
        m_viewModel->removeReceivedRequest(username);
        if (accept){
            m_viewModel->addFriend(username);
        }
    }
    else {
        Logger::error("Error: " + message);
    }
}

void ContactsResponseHandler::handleFriendDeleted(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("to").toString();
        m_viewModel->removeFriend(username);
    } else {
        Logger::error("Error: " + message);
    }
}