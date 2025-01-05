#include "../controllers/utils/Logger.h"
#include "../controllers/contacts/ContactsController.h"
#include "../controllers/session/UserManager.h"

ContactViewModel::ContactViewModel(QObject* parent)
    : QObject(parent)
{
}

// =================== UI Actions ===================
void ContactViewModel::sendFriendRequest(const QString& username) {
    if (m_sentRequests.contains(username)) {
        Logger::debug("Friend request already sent to " + username);
        return;
    }
    else if (m_receivedRequests.contains(username)) {
        Logger::debug("Friend request already received from " + username);
        return;
    }
    else if (m_friends.contains(username)) {
        Logger::debug("Already friends with " + username);
        return;
    }
    else {
        if (username == UserManager::instance()->currentUser()) {
            Logger::error("Cannot send friend request to self");
            return;
        }
        if (username.isEmpty()) {
            Logger::error("Username cannot be empty");
            return;
        }
        Logger::debug("Sending friend request to " + username);
        emit contactActionRequested(ContactAction::SendRequest, username);
    }
}

void ContactViewModel::cancelFriendRequest(const QString& username) {
    Logger::debug("Canceling friend request to " + username);
    emit contactActionRequested(ContactAction::CancelRequest, username);
}

void ContactViewModel::respondFriendRequest(const QString& username, bool accept) {
    Logger::debug(QString("%1 friend request from %2")
                 .arg(accept ? "Accepting" : "Rejecting")
                 .arg(username));
    emit contactActionRequested(ContactAction::RespondRequest, username, accept);
}

void ContactViewModel::deleteFriend(const QString& username) {
    Logger::debug("Removing friend " + username);
    emit contactActionRequested(ContactAction::DeleteFriend, username);
}

void ContactViewModel::fetchFriends() {
    emit contactActionRequested(ContactAction::FetchFriends, UserManager::instance()->currentUser());
}

void ContactViewModel::fetchSentRequests() {
    emit contactActionRequested(ContactAction::FetchSentRequests, UserManager::instance()->currentUser());
}

void ContactViewModel::fetchReceivedRequests() {
    emit contactActionRequested(ContactAction::FetchReceivedRequests, UserManager::instance()->currentUser());
}

// =================== Update ===================
void ContactViewModel::addSentRequest(const QString& username) {
    Logger::debug("Adding sent request to " + username);
    if (!m_sentRequests.contains(username)) {
        m_sentRequests.append(username);
        emit sentRequestsChanged();
    }
}

void ContactViewModel::removeSentRequest(const QString& username) {
    if (m_sentRequests.removeOne(username)) {
        emit sentRequestsChanged();
    }
}

void ContactViewModel::addReceivedRequest(const QString& username) {
    Logger::debug("Adding received request from " + username);
    if (!m_receivedRequests.contains(username)) {
        m_receivedRequests.append(username);
        emit receivedRequestsChanged();
    }
}

void ContactViewModel::removeReceivedRequest(const QString& username) {
    if (m_receivedRequests.removeOne(username)) {
        emit receivedRequestsChanged();
    }
}

void ContactViewModel::addFriend(const QString& username) {
    if (!m_friends.contains(username)) {
        m_friends.append(username);
        emit friendsChanged();
    }
}

void ContactViewModel::removeFriend(const QString& username) {
    if (m_friends.removeOne(username)) {
        emit friendsChanged();
    }
}

// =================== Setter ===================
void ContactViewModel::setFriends(const QStringList& friends){
    m_friends = friends;
    emit friendsChanged();
}

void ContactViewModel::setSentRequests(const QStringList& sentRequests){
    m_sentRequests = sentRequests;
    emit sentRequestsChanged();
}

void ContactViewModel::setReceivedRequests(const QStringList& receivedRequests){
    m_receivedRequests = receivedRequests;
    emit receivedRequestsChanged();
}
