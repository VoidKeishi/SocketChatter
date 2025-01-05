#include "MessageHandler.h"
#include "../utils/Logger.h"
#include "../utils/ResponseFactory.h"

MessageHandler::MessageHandler(DatabaseManager* db) 
    : BaseHandler(nullptr), messageRepo(db) {
    handlers = {
        {"SEND_MESSAGE_RESPONSE", [this](const QJsonObject& p) { handleSendMessage(p); }},
        {"FETCH_MESSAGES_RESPONSE", [this](const QJsonObject& p) { handleFetchMessages(p); }}
    };
}

void MessageHandler::handleSendMessage(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString sender = request["sender"].toString();  
    QString targetContact = payload["contact"].toString();
    QString content = payload["content"].toString();
    qint64 timestamp = request["timestamp"].toInteger();

    Logger::debug(QString("Processing message from %1 to %2").arg(sender).arg(targetContact));

    // Always store the message first
    if (messageRepo.storeMessage(sender, targetContact, content, timestamp)) {
        Logger::info("Message stored in database");
        
        // Then attempt delivery if user is online
        attemptDelivery(targetContact, request);
        
        // Send acknowledgment to sender
        emit responseReady(ResponseFactory::createSendMessageResponse(targetContact, "Message sent"));
    } else {
        Logger::error("Failed to store message in database");
        emit responseReady(ResponseFactory::createSendMessageResponse(targetContact, "Failed to send message"));
    }
}

void MessageHandler::attemptDelivery(const QString& to, const QJsonObject& message) {
    if (activeClients.contains(to)) {
        QTcpSocket* targetSocket = activeClients[to];
        if (targetSocket->state() == QAbstractSocket::ConnectedState) {
            QJsonDocument doc(message);
            QByteArray data = doc.toJson() + '\n';
            targetSocket->write(data);
            targetSocket->flush();
            Logger::info(QString("Message delivered to online user %1").arg(to));
        }
    } else {
        Logger::info(QString("User %1 is offline, message queued").arg(to));
    }
}

void MessageHandler::handleFetchMessages(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString requester = request["sender"].toString();
    QString contact = payload["contact"].toString();
    
    Logger::debug(QString("Fetching messages between %1 and %2")
                 .arg(requester).arg(contact));

    // Fetch messages from database
    QJsonArray messages = messageRepo.getMessages(requester, contact);
    QString messagesString = QString(QJsonDocument(messages).toJson(QJsonDocument::Compact));

    
    // Send messages back to requester
    QJsonObject response = ResponseFactory::createFetchMessagesResponse(contact, messagesString);
    emit responseReady(response);
}

void MessageHandler::registerClient(const QString& username, QTcpSocket* socket) {
    activeClients[username] = socket;
    Logger::info(QString("Registered client: %1").arg(username));
    
    // Fetch and deliver any pending messages
    QJsonArray pendingMessages = messageRepo.getPendingMessages(username);
    for (const QJsonValue& msg : pendingMessages) {
        attemptDelivery(username, msg.toObject());
    }
}

void MessageHandler::removeClient(const QString& username) {
    activeClients.remove(username);
    Logger::info(QString("Removed client: %1").arg(username));
}