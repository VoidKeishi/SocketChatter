#include "MessageHandler.h"
#include "../utils/Logger.h"
#include "../utils/ResponseFactory.h"

MessageHandler::MessageHandler(DatabaseManager* db) 
    : BaseHandler(nullptr), messageRepo(db) {
    handlers = {
        {"SEND_MESSAGE_REQUEST", [this](const QJsonObject& p) { handleSendMessage(p); }},
        {"FETCH_MESSAGES_REQUEST", [this](const QJsonObject& p) { handleFetchMessages(p); }}
    };
}

void MessageHandler::handleSendMessage(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString sender = payload["sender"].toString();  // Changed from request to payload
    QString receiver = payload["receiver"].toString(); // Changed from targetContact
    QString content = payload["content"].toString();
    qint64 timestamp = request["timestamp"].toInteger();

    Logger::debug(QString("Processing message from %1 to %2").arg(sender).arg(receiver));

    // store the message first
    if (messageRepo.storeMessage(sender, receiver, content, timestamp)) {
        Logger::info("Message stored in database");
        
        // if user is online
        attemptDelivery(receiver, request);
        
        // ack sender
        emit responseReady(ResponseFactory::createSendMessageResponse(sender, receiver, content));
    } else {
        Logger::error("Failed to store message in database");
        emit responseReady(ResponseFactory::createSendMessageResponse(sender, receiver, "Failed to send message"));
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