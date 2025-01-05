#include "MessageRepository.h"

MessageRepository::MessageRepository(DatabaseManager* db) : m_db(db) {}

bool MessageRepository::storeMessage(const QString& sender, const QString& receiver, 
                                   const QString& content, qint64 timestamp) {
    QSqlQuery query(m_db->database());
    query.prepare("INSERT INTO messages (sender, receiver, content, timestamp) "
                 "VALUES (:sender, :receiver, :content, :timestamp)");
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", receiver);
    query.bindValue(":content", content);
    query.bindValue(":timestamp", timestamp);
    
    return query.exec();
}

QJsonArray MessageRepository::getMessages(const QString& sender, const QString& receiver) {
    QJsonArray messages;
    QSqlQuery query(m_db->database());
    
    query.prepare("SELECT id, sender, receiver, content, timestamp FROM messages "
                 "WHERE (sender = :sender AND receiver = :receiver) "
                 "OR (sender = :receiver AND receiver = :sender) "
                 "ORDER BY timestamp ASC");
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", receiver);
    
    if (query.exec()) {
        while (query.next()) {
            QJsonObject message{
                {"id", query.value("id").toString()},
                {"sender", query.value("sender").toString()},
                {"receiver", query.value("receiver").toString()},
                {"content", query.value("content").toString()},
                {"timestamp", query.value("timestamp").toLongLong()}
            };
            messages.append(message);
        }
    }
    
    return messages;
}

QJsonArray MessageRepository::getPendingMessages(const QString& username) {
    QJsonArray messages;
    QSqlQuery query(m_db->database());
    query.prepare("SELECT id, sender, receiver, content, timestamp FROM messages "
                 "WHERE receiver = :username AND delivered = false "
                 "ORDER BY timestamp ASC");
    query.bindValue(":username", username);
    
    if (query.exec()) {
        while (query.next()) {
            QJsonObject message{
                {"id", query.value("id").toString()},
                {"sender", query.value("sender").toString()},
                {"receiver", query.value("receiver").toString()},
                {"content", query.value("content").toString()},
                {"timestamp", query.value("timestamp").toLongLong()}
            };
            messages.append(message);
        }
    }
    
    return messages;
}