#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager(QObject* parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chatapp.db");

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    } else {
        initializeDatabase();
    }

    // For testing, pre-populate with a user
    users["testuser"] = "testpasshash";
}

void DatabaseManager::initializeDatabase() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS Users ("
               "user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT UNIQUE NOT NULL, "
               "password TEXT NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS Friends ("
               "user_id INTEGER, "
               "friend_id INTEGER, "
               "status TEXT NOT NULL, "
               "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
               "FOREIGN KEY(user_id) REFERENCES Users(user_id), "
               "FOREIGN KEY(friend_id) REFERENCES Users(user_id), "
               "PRIMARY KEY(user_id, friend_id))");
}

bool DatabaseManager::authenticateUser(const QString& username, const QString& passwordHash) {
    QSqlQuery query;
    query.prepare("SELECT password FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return query.value(0).toString() == passwordHash;
    }
    return false;
}

bool DatabaseManager::registerUser(const QString& username, const QString& passwordHash) {
    QSqlQuery query;
    query.prepare("INSERT INTO Users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", passwordHash);

    if (!query.exec()) {
        qWarning() << "Failed to register user:" << query.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::addUserSocket(const QString& username, QTcpSocket* socket) {
    userSockets.insert(username, socket);
    socketUsers.insert(socket, username);
}

void DatabaseManager::removeUserSocket(const QString& username) {
    QTcpSocket* socket = userSockets.value(username, nullptr);
    if (socket) {
        socketUsers.remove(socket);
    }
    userSockets.remove(username);
}

QTcpSocket* DatabaseManager::getUserSocket(const QString& username) {
    return userSockets.value(username, nullptr);
}

QString DatabaseManager::getUsernameBySocket(QTcpSocket* socket) {
    return socketUsers.value(socket, QString());
}

bool DatabaseManager::sendFriendRequest(const QString& sender, const QString& receiver) {
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Friends WHERE user_id = (SELECT user_id FROM Users WHERE username = :sender) AND friend_id = (SELECT user_id FROM Users WHERE username = :receiver)");
    checkQuery.bindValue(":sender", sender);
    checkQuery.bindValue(":receiver", receiver);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qWarning() << "Failed to check existing friend request:" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.value(0).toInt() > 0) {
        qWarning() << "Friend request already exists between" << sender << "and" << receiver;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Friends (user_id, friend_id, status) VALUES ((SELECT user_id FROM Users WHERE username = :sender), (SELECT user_id FROM Users WHERE username = :receiver), 'pending')");
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", receiver);

    if (!query.exec()) {
        qWarning() << "Failed to send friend request:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::respondToFriendRequest(const QString& sender, const QString& receiver, bool accepted) {
    QSqlQuery query;
    QString status = accepted ? "accepted" : "denied";
    query.prepare("UPDATE Friends SET status = :status WHERE user_id = (SELECT user_id FROM Users WHERE username = :sender) AND friend_id = (SELECT user_id FROM Users WHERE username = :receiver)");
    query.bindValue(":status", status);
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", receiver);

    if (!query.exec()) {
        qWarning() << "Failed to respond to friend request:" << query.lastError().text();
        return false;
    }
    return true;
}

QStringList DatabaseManager::getSentFriendRequests(const QString& username) {
    QStringList sentRequests;
    QSqlQuery query;
    query.prepare("SELECT username FROM Users WHERE user_id IN (SELECT friend_id FROM Friends WHERE user_id = (SELECT user_id FROM Users WHERE username = :username) AND status = 'pending')");
    query.bindValue(":username", username);

    if (query.exec()) {
        while (query.next()) {
            sentRequests << query.value(0).toString();
        }
    } else {
        qWarning() << "Failed to get sent friend requests:" << query.lastError().text();
    }
    return sentRequests;
}

QStringList DatabaseManager::getReceivedFriendRequests(const QString& username) {
    QStringList receivedRequests;
    QSqlQuery query;
    query.prepare("SELECT username FROM Users WHERE user_id IN (SELECT user_id FROM Friends WHERE friend_id = (SELECT user_id FROM Users WHERE username = :username) AND status = 'pending')");
    query.bindValue(":username", username);

    if (query.exec()) {
        while (query.next()) {
            receivedRequests << query.value(0).toString();
        }
    } else {
        qWarning() << "Failed to get received friend requests:" << query.lastError().text();
    }
    return receivedRequests;
}

QStringList DatabaseManager::getContactList(const QString& username) {
    QStringList contacts;
    QSqlQuery query;
    query.prepare("SELECT username FROM Users WHERE user_id IN (SELECT friend_id FROM Friends WHERE user_id = (SELECT user_id FROM Users WHERE username = :username) AND status = 'accepted')");
    query.bindValue(":username", username);

    if (query.exec()) {
        while (query.next()) {
            contacts << query.value(0).toString();
        }
    } else {
        qWarning() << "Failed to get contact list:" << query.lastError().text();
    }
    return contacts;
}