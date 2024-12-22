#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "../database/DatabaseManager.h"
#include <QString>
#include <QSqlQuery>

class UserRepository {
public:
    explicit UserRepository(DatabaseManager* db);
    
    bool createUser(const QString& username, const QString& passwordHash);
    bool validateUser(const QString& username, const QString& passwordHash);
    bool userExists(const QString& username);

private:
    DatabaseManager* db;
};

#endif // USER_REPOSITORY_H