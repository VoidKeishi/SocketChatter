// src/controllers/network/ResponseManager.h
#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

#include <QObject>
#include <QStringList>

class ResponseManager : public QObject {
    Q_OBJECT

public:
    static ResponseManager* instance();

signals:
    void loginResponseReceived(bool success, const QString &message);
    void registerResponseReceived(bool success, const QString &message);

    // New signals for ManageContactsController
    void friendRequestResponseReceived(bool success, const QString &message);
    void cancelFriendRequestResponseReceived(bool success, const QString &message);
    void acceptFriendRequestResponseReceived(bool success, const QString &message);
    void rejectFriendRequestResponseReceived(bool success, const QString &message);
    void deleteFriendResponseReceived(bool success, const QString &message);
    void friendsListReceived(const QStringList &friends);

private slots:
    void onDataReceived(const QByteArray &data);

private:
    explicit ResponseManager(QObject *parent = nullptr);
    static ResponseManager* m_instance;
};

#endif // RESPONSEMANAGER_H