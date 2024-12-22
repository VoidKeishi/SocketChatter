#ifndef CONTACTSCONTROLLER_H
#define CONTACTSCONTROLLER_H

#include <QObject>
#include <QStringList>

class ContactsController : public QObject {
    Q_OBJECT

public:
    explicit ContactsController(QObject *parent = nullptr);

    // Methods to handle UI actions
    Q_INVOKABLE void requestAddFriend(const QString &username);
    Q_INVOKABLE void requestCancelFriend(const QString &username);
    Q_INVOKABLE void requestRespondToFriend(const QString &username, bool accept);
    Q_INVOKABLE void requestDeleteFriend(const QString &username);
    Q_INVOKABLE void requestFetchFriends();

public slots:
    // Slots to handle responses from server
    void handleFriendRequestSent(const QJsonObject &response);
    void handleFriendRequestCanceled(const QJsonObject &response);
    void handleFriendRequestResponse(const QJsonObject &response);
    void handleFriendsListFetched(const QJsonObject &response);
    void handleFriendDeleted(const QJsonObject &response);

signals:
    // Signals to send requests to server
    void sendAddFriend(const QByteArray &data);
    void sendCancelFriend(const QByteArray &data);
    void sendRespondToFriend(const QByteArray &data);
    void sendDeleteFriend(const QByteArray &data);
    void sendFetchFriends(const QByteArray &data);

    // Signals to inform UI of results
    void addFriendResult(bool success, const QString &message);
    void cancelFriendResult(bool success, const QString &message);
    void friendResponseResult(bool success, const QString &message);
    void friendDeletedResult(bool success, const QString &message);
    void friendsListResult(const QStringList &friends);
};

#endif // CONTACTSCONTROLLER_H