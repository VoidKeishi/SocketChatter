#pragma once
#include "../database/DatabaseManager.h"
#include "../utils/Logger.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>

class MessageRepository {
public:
    explicit MessageRepository(DatabaseManager* db);
    
    bool storeMessage(const QString& sender, const QString& receiver, 
                     const QString& content, qint64 timestamp);
    QJsonArray getMessages(const QString& sender, const QString& receiver);
    QJsonArray getPendingMessages(const QString& username);

private:
    DatabaseManager* m_db;
};