#include "ContactsController.h"
#include "../utils/RequestFactory.h"
#include "../session/UserManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

ContactsController::ContactsController(ContactViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
    handlers = {
        {"FRIEND_REQUEST_SENT_RESPONSE", [this](const QJsonObject& p) { handleFriendRequestSent(p); }},
        {"FRIEND_REQUEST_CANCEL_RESPONSE", [this](const QJsonObject& p) { handleFriendRequestCanceled(p); }},
        {"FRIEND_REQUEST_RESPONSE", [this](const QJsonObject& p) { handleFriendRequestResponse(p); }},
        {"FRIEND_DELETED_RESPONSE", [this](const QJsonObject& p) { handleFriendDeleted(p); }},
        {"FETCH_FRIEND_LIST_RESPONSE", [this](const QJsonObject& p) { handleFriendsListFetched(p); }},
        {"FETCH_SENT_REQUESTS_RESPONSE", [this](const QJsonObject& p) { handleSentListFetched(p); }},
        {"FETCH_RECEIVED_REQUESTS_RESPONSE", [this](const QJsonObject& p) { handleReceivedListFetched(p); }},
    };
    connect(viewModel, &ContactViewModel::contactActionRequested,
        this, &ContactsController::handleContactAction);
}

bool ContactsController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

void ContactsController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        qDebug() << "Handling: " << type;
        handlers[type](payload);
    }
    else {
        qDebug() << "Missing handler for: " << type;
    }
}

void ContactsController::handleContactAction(ContactAction action, const QString& username, bool param) {
    switch (action) {
    case ContactAction::SendRequest:
        requestAddFriend(username);
        break;
    case ContactAction::CancelRequest:
        requestCancelFriend(username);
        break;
    case ContactAction::RespondRequest:
        requestRespondToFriend(username, param);
        break;
    case ContactAction::DeleteFriend:
        requestDeleteFriend(username);
        break;
    case ContactAction::FetchFriends:
        requestFetchFriends();
        break;
    case ContactAction::FetchSentRequests:
        requestFetchSentRequests();
        break;
    case ContactAction::FetchReceivedRequests:
        requestFetchReceivedRequests();
        break;
    }
}   

// Methods to handle UI actions
void ContactsController::requestAddFriend(const QString &to) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createSendFriendRequest(from, to);
    emit sendRequest(requestData);
}

void ContactsController::requestCancelFriend(const QString &username) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createCancelFriendRequest(from, username);
    emit sendRequest(requestData);
}

void ContactsController::requestRespondToFriend(const QString &username, bool accept) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createResponseFriendRequest(from, username, accept);
    emit sendRequest(requestData);
}

void ContactsController::requestDeleteFriend(const QString &username) {
    QString from = UserManager::instance()->currentUser();
    QByteArray requestData = RequestFactory::createDeleteFriendRequest(from, username);
    emit sendRequest(requestData);
}

void ContactsController::requestFetchFriends() {
    QByteArray requestData = RequestFactory::createFetchFriendsListRequest();
    emit sendRequest(requestData);
}

void ContactsController::requestFetchSentRequests() {
    QByteArray requestData = RequestFactory::createFetchSentRequestsRequest();
    emit sendRequest(requestData);
}

void ContactsController::requestFetchReceivedRequests() {
    QByteArray requestData = RequestFactory::createFetchReceivedRequestsRequest();
    emit sendRequest(requestData);
}

// Slots to handle fetch response
void ContactsController::handleFriendsListFetched(const QJsonObject& response) {
    QStringList friends;
    QJsonArray friendsArray = response.value("friends").toArray();
    for (const QJsonValue& value : friendsArray) {
        friends << value.toString();
    }
    m_viewModel->setFriends(friends);
}

void ContactsController::handleSentListFetched(const QJsonObject& response) {
    QStringList sentRequests;
    QJsonArray sentArray = response.value("sent").toArray();
    for (const QJsonValue& value : sentArray) {
        sentRequests << value.toString();
    }
    m_viewModel->setSentRequests(sentRequests);
    for (const auto& request : sentRequests) {
        qDebug() << "Request: " << request;
    }
}

void ContactsController::handleReceivedListFetched(const QJsonObject& response) {
    QStringList receivedRequests;
    QJsonArray receivedArray = response.value("requests").toArray();
    for (const QJsonValue& value : receivedArray) {
        receivedRequests << value.toString(); 
    }
    m_viewModel->setReceivedRequests(receivedRequests);
}

// Slots to handle update responses
void ContactsController::handleFriendRequestSent(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("to").toString();
        m_viewModel->addSentRequest(username);
        qDebug () << "Friend request sent success: " << username;
    } else {
        qDebug() << "Error: " << message;
    }
}

void ContactsController::handleFriendRequestCanceled(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("to").toString();
        m_viewModel->removeSentRequest(username);
        qDebug() << "Friend request canceled: " << username;
    } else {
        qDebug() << "Error: " << message;
    }
}

void ContactsController::handleFriendRequestResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    QString username = response.value("to").toString();
    
    if (success) {
        m_viewModel->removeReceivedRequest(username);
        m_viewModel->addFriend(username);
    }
    else {
        qDebug() << "Error: " << message;
    }
}

void ContactsController::handleFriendDeleted(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    QString message = response.value("message").toString();
    
    if (success) {
        QString username = response.value("to").toString();
        m_viewModel->removeFriend(username);
    } else {
        qDebug() << "Error: " << message;
    }
}