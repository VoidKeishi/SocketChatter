#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>

class RequestManager : public QObject {
    Q_OBJECT
public:
    static RequestManager* instance();
    explicit RequestManager(QObject *parent = nullptr);
    void sendLoginRequest(const QString &username, const QString &password);
    void sendRegisterRequest(const QString &username, const QString &password);
    void sendFriendRequest(const QString &username);
    void cancelFriendRequest(const QString &username);
    void acceptFriendRequest(const QString &username);
    void rejectFriendRequest(const QString &username);
    void deleteFriend(const QString &username);
    void getFriendsList();

private:
    static RequestManager* m_instance;
    QString hashPassword(const QString &password);
    void sendRequest(const QString &type, const QJsonObject &payload);
};

#endif // REQUESTMANAGER_H