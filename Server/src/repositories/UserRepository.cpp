#include "UserRepository.h"

UserRepository::UserRepository(DatabaseManager* db) : db(db) {}

bool UserRepository::createUser(const QString& username, const QString& passwordHash) {
    QSqlQuery query(db->database());
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(passwordHash);
    return query.exec();
}

bool UserRepository::validateUser(const QString& username, const QString& passwordHash) {
    QSqlQuery query(db->database());
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec() || !query.next()) {
        return false;
    }
    return query.value(0).toString() == passwordHash;
}

bool UserRepository::userExists(const QString& username) {
    QSqlQuery query(db->database());
    query.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    query.addBindValue(username);
    return query.exec() && query.next() && query.value(0).toInt() > 0;
}