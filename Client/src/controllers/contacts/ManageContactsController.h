// src/controllers/ManageContactsController.h
#ifndef MANAGECONTACTSCONTROLLER_H
#define MANAGECONTACTSCONTROLLER_H

#include <QObject>
#include <QStringList>

class ManageContactsController : public QObject {
    Q_OBJECT

public:
    explicit ManageContactsController(QObject *parent = nullptr);

    // Methods to send friend requests, accept/reject requests, delete friends, etc.
    void sendFriendRequest(const QString &username);
    void cancelFriendRequest(const QString &username);
    void acceptFriendRequest(const QString &username);
    void rejectFriendRequest(const QString &username);
    void deleteFriend(const QString &username);
    void fetchFriendsList();

signals:
    void friendRequestSent(bool success, const QString &message);
    void friendRequestCanceled(bool success, const QString &message);
    void friendRequestAccepted(bool success, const QString &message);
    void friendRequestRejected(bool success, const QString &message);
    void friendDeleted(bool success, const QString &message);
    void friendsListFetched(const QStringList &friends);
};

#endif // MANAGECONTACTSCONTROLLER_H