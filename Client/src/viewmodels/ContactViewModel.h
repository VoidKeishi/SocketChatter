#pragma once

#include <QObject>
#include <QStringList>

enum class ContactAction {
    SendRequest,
    CancelRequest,
    RespondRequest,
    DeleteFriend,
    FetchFriends,
    FetchSentRequests,
    FetchReceivedRequests,
};



class ContactViewModel : public QObject {
    Q_OBJECT
    // Sent requests
    Q_PROPERTY(QStringList sentRequests READ sentRequests NOTIFY sentRequestsChanged)
    // Received requests
    Q_PROPERTY(QStringList receivedRequests READ receivedRequests NOTIFY receivedRequestsChanged)
    // Accepted friends
    Q_PROPERTY(QStringList friends READ friends NOTIFY friendsChanged)

public:
    explicit ContactViewModel(QObject* parent = nullptr);
    // Add getters for properties (required for QML)
    QStringList sentRequests() const { return m_sentRequests; }
    QStringList receivedRequests() const { return m_receivedRequests; }
    QStringList friends() const { return m_friends; }

    // Updaters
    Q_INVOKABLE void addSentRequest(const QString& username);
    Q_INVOKABLE void removeSentRequest(const QString& username);
    Q_INVOKABLE void addReceivedRequest(const QString& username);
    Q_INVOKABLE void removeReceivedRequest(const QString& username);
    Q_INVOKABLE void addFriend(const QString& username);
    Q_INVOKABLE void removeFriend(const QString& username);

    // Setter
    void setFriends(const QStringList& friends);
    void setSentRequests(const QStringList& sentRequests);
    void setReceivedRequests(const QStringList& receivedRequests);

public slots:
    // Methods called by View
    void sendFriendRequest(const QString& username);
    void cancelFriendRequest(const QString& username); 
    void respondFriendRequest(const QString& username, bool accept);
    void deleteFriend(const QString& username);
    void fetchFriends();
    void fetchSentRequests();
    void fetchReceivedRequests();

signals:
    void contactActionRequested(ContactAction action, const QString& username, bool param = false);
    void sentRequestsChanged();
    void receivedRequestsChanged();
    void friendsChanged();
    void loadingChanged();
    void statusChanged();

private:
    QStringList m_sentRequests;
    QStringList m_receivedRequests;
    QStringList m_friends;

    QStringList m_groupInvitations;
    QStringList m_groups;

    QString m_statusMessage;
};