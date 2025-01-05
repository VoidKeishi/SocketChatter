#include "MessageHandler.h"
#include "../utils/Logger.h"

MessageHandler::MessageHandler(QObject* parent) : BaseHandler(parent) {}

void MessageHandler::handle(const QString& type, const QJsonObject& request) {
    Logger::debug("MessageHandler: Starting to handle message");  // Add this
    if (type != "SEND_MESSAGE") return;
    
    auto payload = request["payload"].toObject();
    QString targetContact = payload["contact"].toString();
    Logger::debug(QString("MessageHandler: Attempting to send to %1").arg(targetContact));  // Add this
    
    // Log active clients
    Logger::debug(QString("MessageHandler: Currently registered clients: %1")
        .arg(activeClients.keys().join(", ")));  // Add this
    
    if (!activeClients.contains(targetContact)) {
        Logger::error(QString("Target user %1 not found").arg(targetContact));
        emit error(QString("User %1 not found or offline").arg(targetContact));
        return;
    }
    
    // Create message to forward
    QJsonObject messageToForward;
    messageToForward["type"] = "RECEIVED_MESSAGE";
    messageToForward["payload"] = payload;
    messageToForward["timestamp"] = request["timestamp"];
    
    QJsonDocument doc(messageToForward);
    QByteArray data = doc.toJson() + '\n';  
    
    // Send to recipient
    QTcpSocket* targetSocket = activeClients[targetContact];
    if (targetSocket->state() == QAbstractSocket::ConnectedState) {
        targetSocket->write(data);
        targetSocket->flush();
        Logger::info(QString("Message forwarded to %1").arg(targetContact));
    } else {
        Logger::error(QString("Target user %1 socket not connected").arg(targetContact));
        emit error("Target user socket not connected");
    }
}

void MessageHandler::registerClient(const QString& username, QTcpSocket* socket) {
    activeClients[username] = socket;
    Logger::info(QString("MessageHandler: Registered client: %1 (Total clients: %2)")
        .arg(username)
        .arg(activeClients.size()));
}

void MessageHandler::removeClient(const QString& username) {
    activeClients.remove(username);
    Logger::info(QString("Removed client: %1").arg(username));
}