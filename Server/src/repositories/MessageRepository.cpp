#include "MessageRepository.h"

MessageRepository::MessageRepository(DatabaseManager* db) : m_db(db) {}

bool MessageRepository::storeMessage(const QString& sender, const QString& receiver, const QString& content, qint64 timestamp) {
    QSqlQuery query(m_db->database());
    Logger::debug("storeMessage()"); 
    Logger::debug(QString("Attempting to store message:"));
    Logger::debug(QString("Sender: %1").arg(sender));
    Logger::debug(QString("Receiver: %1").arg(receiver));
    Logger::debug(QString("Content: %1").arg(content));
    Logger::debug(QString("Timestamp: %1").arg(timestamp));

    const QString sql = "INSERT INTO messages (sender, receiver, content, timestamp) "
                       "VALUES (?, ?, ?, ?)";
    
    Logger::debug(QString("SQL Query: %1").arg(sql));
    query.prepare(sql);
    
    query.addBindValue(sender);
    query.addBindValue(receiver);
    query.addBindValue(content);
    query.addBindValue(timestamp);

    query.exec("SELECT * FROM messages");
    // QString debugQuery = "INSERT INTO messages (sender, receiver, content, timestamp) "
    //                     "VALUES ('%1', '%2', '%3', %4)";
    // debugQuery = debugQuery.arg(sender)
    //                       .arg(receiver)
    //                       .arg(content)
    //                       .arg(timestamp);
    
    // Logger::debug(QString("Actual query to be executed: %1").arg(debugQuery));

    
    
    if (!query.exec()) {
        Logger::error(QString("Database error: %1").arg(query.lastError().text()));
        Logger::error(QString("Database error type: %1").arg(query.lastError().type()));
        Logger::error(QString("Database driver error: %1").arg(query.lastError().databaseText()));
        Logger::error(QString("Driver error: %1").arg(query.lastError().driverText()));
        return false;
    }
    
    return true;
}

QJsonArray MessageRepository::getMessages(const QString& sender, const QString& receiver) {
    QJsonArray messages;
    QSqlQuery query(m_db->database());
    Logger::debug("getMessages()"); 
    query.prepare("SELECT id, sender, receiver, content, timestamp FROM messages "
                 "WHERE (sender = ? AND receiver = ?) "
                 "OR (sender = ? AND receiver = ?) "
                 "ORDER BY timestamp ASC");
    query.addBindValue(sender);
    query.addBindValue(receiver);
    query.addBindValue(receiver);
    query.addBindValue(sender);
    
    if (query.exec()) {
        while (query.next()) {
            QJsonObject message{
                {"id", query.value(0).toString()},
                {"sender", query.value(1).toString()},
                {"receiver", query.value(2).toString()},
                {"content", query.value(3).toString()},
                {"timestamp", query.value(4).toLongLong()}
            };
            messages.append(message);
        }
    }
    
    return messages;
}

QJsonArray MessageRepository::getPendingMessages(const QString& username) {
    QJsonArray messages;
    QSqlQuery query(m_db->database());
    Logger::debug("getPendingMessages()"); 
    query.prepare("SELECT id, sender, receiver, content, timestamp FROM messages "
                 "WHERE receiver = ? "
                 "ORDER BY timestamp ASC");
    query.addBindValue(username);
    
    if (query.exec()) {
        while (query.next()) {
            QJsonObject message{
                {"id", query.value(0).toString()},
                {"sender", query.value(1).toString()},
                {"receiver", query.value(2).toString()},
                {"content", query.value(3).toString()},
                {"timestamp", query.value(4).toLongLong()}
            };
            messages.append(message);
        }
    }
    
    return messages;
}