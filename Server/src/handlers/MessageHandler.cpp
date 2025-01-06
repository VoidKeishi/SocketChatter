#include "MessageHandler.h"
#include "../core/ConnectionManager.h"
#include "../utils/Logger.h"
#include "../utils/ResponseFactory.h"
#include "../utils/NotificationFactory.h"

MessageHandler::MessageHandler(DatabaseManager* db) 
    : BaseHandler(nullptr), messageRepo(db) {
    handlers = {
        {"SEND_MESSAGE_REQUEST", [this](const QJsonObject& p) { handleSendMessage(p); }},
        {"FETCH_MESSAGES_REQUEST", [this](const QJsonObject& p) { handleFetchMessages(p); }}
    };
}

void MessageHandler::handleSendMessage(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString sender = payload["sender"].toString(); 
    QString receiver = payload["receiver"].toString();
    QString content = payload["content"].toString();
    qint64 timestamp = request["timestamp"].toInteger();

    Logger::debug(QString("Processing message from %1 to %2").arg(sender).arg(receiver));

    // store the message first
    if (messageRepo.storeMessage(sender, receiver, content, timestamp)) {
        Logger::info("Message stored in database");
        QJsonObject messageNotification = NotificationFactory::createMessageNotification(sender, receiver, content);
        // Notify the friend that they have been deleted
        ConnectionManager::instance()->sendMessageToClient(receiver, messageNotification );
        // ack sender
        emit responseReady(ResponseFactory::createSendMessageResponse(sender, receiver, content));
    } else {
        Logger::error("Failed to store message in database");
        emit responseReady(ResponseFactory::createSendMessageResponse(sender, receiver, "Failed to send message"));
    }
}

void MessageHandler::handleFetchMessages(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString sender = payload["sender"].toString();
    QString receiver = payload["receiver"].toString();
    
    Logger::debug(QString("Fetching messages between %1 and %2")
                 .arg(sender).arg(receiver));

    // fetch for new msg
    QJsonArray messages = messageRepo.getMessages(sender, receiver);
    QString messagesString = QString(QJsonDocument(messages).toJson(QJsonDocument::Compact));
    
    // send the fetched data to user
    QJsonObject response = ResponseFactory::createFetchMessagesResponse(sender, receiver, messagesString);
    emit responseReady(response);
}