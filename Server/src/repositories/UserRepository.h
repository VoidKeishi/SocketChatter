#pragma once

#include <QString>
#include <QSqlQuery>

#include "../database/DatabaseManager.h"

class UserRepository {
public:
    explicit UserRepository(DatabaseManager* db);
    
    bool createUser(const QString& username, const QString& passwordHash);
    bool validateUser(const QString& username, const QString& passwordHash);
    bool userExists(const QString& username);

private:
    DatabaseManager* db;
};
