#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QMap>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QStringList>

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    static DatabaseManager& instance();

    void initializeDatabase();    

    bool authenticateUser(const QString& username, const QString& passwordHash);
    bool registerUser(const QString& username, const QString& passwordHash);
    void addUserSocket(const QString& username, QTcpSocket* socket);
    void removeUserSocket(const QString& username);
    QTcpSocket* getUserSocket(const QString& username);
    QString getUsernameBySocket(QTcpSocket* socket);

    // Friend request methods
    bool sendFriendRequest(const QString& sender, const QString& receiver);
    bool respondToFriendRequest(const QString& sender, const QString& receiver, bool accepted);
    QStringList getSentFriendRequests(const QString& username);
    QStringList getReceivedFriendRequests(const QString& username);
    QStringList getContactList(const QString& username); // Add this line


private:
    explicit DatabaseManager(QObject* parent = nullptr);
    QSqlDatabase db;

    QMap<QString, QString> users; // Username -> PasswordHash
    QMap<QString, QTcpSocket*> userSockets; // Username -> Socket
    QMap<QTcpSocket*, QString> socketUsers; // Socket -> Username
};

#endif // DATABASEMANAGER_H