#pragma once

#include <QObject>
#include <QString>

class ContactsRequestSender : public QObject {
    Q_OBJECT

public:
    explicit ContactsRequestSender(QObject* parent = nullptr);

    void requestAddFriend(const QString& from, const QString& to);
    void requestCancelFriend(const QString& from, const QString& to);
    void requestRespondToFriend(const QString& from, const QString& to, bool accept);
    void requestDeleteFriend(const QString& from, const QString& to);
    void requestFetchFriends(const QString& username);
    void requestFetchSentRequests(const QString& username);
    void requestFetchReceivedRequests(const QString& username);

signals:
    void sendRequest(const QByteArray& data);
};