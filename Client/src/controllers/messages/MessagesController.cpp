#include "MessagesController.h"

MessagesController::MessagesController(ConversationViewModel* viewModel, QObject* parent)
    : QObject(parent), m_viewModel(viewModel) {
    handlers = {
        {"SEND_MESSAGE_RESPONSE", [this](const QJsonObject& p) { handleSendMessageResponse(p); }},
        {"FETCH_MESSAGES_RESPONSE", [this](const QJsonObject& p) { handleFetchMessagesResponse(p); }}
    };

    connect(m_viewModel, &ConversationViewModel::sendMessageRequested, 
            this, &MessagesController::sendMessage);
    connect(m_viewModel, &ConversationViewModel::fetchMessagesRequested, 
            this, &MessagesController::fetchMessages);
    connect(this, &MessagesController::sendRequest, 
            NetworkController::instance(), &NetworkController::sendData);
}

bool MessagesController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

void MessagesController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        handlers[type](payload);
    }
}

void MessagesController::sendMessage(const QString& sender, const QString& receiver, const QString& content) {
    QByteArray requestData = RequestFactory::createSendMessageRequest(sender, receiver, content);
    emit sendRequest(requestData);
}

void MessagesController::fetchMessages(const QString& sender, const QString& receiver) {
    QByteArray requestData = RequestFactory::createFetchMessagesRequest(sender, receiver);
    emit sendRequest(requestData);
}

void MessagesController::handleSendMessageResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    if (success) {
        QString sender = response.value("sender").toString();
        QString receiver = response.value("receiver").toString();
        QString content = response.value("content").toString();
        QString id = response.value("id").toString();
        QDateTime timestamp = QDateTime::fromString(response.value("timestamp").toString(), Qt::ISODate);
        m_viewModel->onMessageReceived(sender, receiver, content, timestamp, id);
    } else {
        Logger::error("Failed to send message");
    }
}

void MessagesController::handleFetchMessagesResponse(const QJsonObject& response) {
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