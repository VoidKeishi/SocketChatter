#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#include "ContactsController.h"
#include "ContactsRequestSender.h"
#include "ContactsResponseHandler.h"
#include "../utils/RequestFactory.h"
#include "../utils/Logger.h"
#include "../session/UserManager.h"

ContactsController::ContactsController(ContactViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
    , m_requestSender(new ContactsRequestSender(this))
    , m_responseHandler(new ContactsResponseHandler(viewModel, this))
{
    handlers = {
        {"FRIEND_REQUEST_SENT_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleFriendRequestSent(p); }},
        {"FRIEND_REQUEST_CANCEL_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleFriendRequestCanceled(p); }},
        {"FRIEND_REQUEST_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleFriendRequestResponse(p); }},
        {"FRIEND_DELETED_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleFriendDeleted(p); }},
        {"FETCH_FRIEND_LIST_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleFriendsListFetched(p); }},
        {"FETCH_SENT_REQUESTS_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleSentListFetched(p); }},
        {"FETCH_RECEIVED_REQUESTS_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleReceivedListFetched(p); }},
    };
    connect(viewModel, &ContactViewModel::contactActionRequested,
        this, &ContactsController::handleContactAction);
}

bool ContactsController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

void ContactsController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        handlers[type](payload);
    }
}

void ContactsController::handleContactAction(ContactAction action, const QString& username, bool param) {
    QString currentUser = UserManager::instance()->currentUser();
    if (currentUser.isEmpty()) {
        Logger::error("Cannot perform contact action: No user logged in");
        return;
    }

    switch (action) {
    case ContactAction::SendRequest:
        m_requestSender->requestAddFriend(currentUser, username);
        break;
    case ContactAction::CancelRequest:
        m_requestSender->requestCancelFriend(currentUser, username);
        break;
    case ContactAction::RespondRequest:
        m_requestSender->requestRespondToFriend(currentUser, username, param);
        break;
    case ContactAction::DeleteFriend:
        m_requestSender->requestDeleteFriend(currentUser, username);
        break;
    case ContactAction::FetchFriends:
        m_requestSender->requestFetchFriends(currentUser);
        break;
    case ContactAction::FetchSentRequests:
        m_requestSender->requestFetchSentRequests(currentUser);
        break;
    case ContactAction::FetchReceivedRequests:
        m_requestSender->requestFetchReceivedRequests(currentUser);
        break;
    }
}   