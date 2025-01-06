#include <QJsonArray>

#include "MessagesResponseHandler.h"
#include "../utils/Logger.h"


MessagesResponseHandler::MessagesResponseHandler(ConversationViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
}

// Slots to handle fetch response
void MessagesResponseHandler::handleSendMessageResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    if (success) { 
        QString sender = response.value("sender").toString();
        QString receiver = response.value("receiver").toString();
        QString content = response.value("content").toString();
        QString id = response.value("id").toString();
        QDateTime timestamp = QDateTime::fromString(response.value("timestamp").toString(), Qt::ISODate);
        m_viewModel->onMessageAckReceived(sender, receiver, content, timestamp, id);
    } else {
        Logger::error("Failed to send message"); 
    }
}

void MessagesResponseHandler::handleFetchMessagesResponse(const QJsonObject& response) {
    QJsonArray messagesArray = response.value("messages").toArray();
    QVector<Message> messages;
    for (const QJsonValue& value : messagesArray) {
        QJsonObject obj = value.toObject();
        messages.append({
            obj.value("id").toString(),
            obj.value("sender").toString(),
            obj.value("receiver").toString(),
            obj.value("content").toString(),
            QDateTime::fromString(obj.value("timestamp").toString(), Qt::ISODate)
        });
    }
    m_viewModel->onMessagesFetched(messages);
}

