#include <QJsonArray>

#include "MessagesController.h"
#include "../utils/RequestFactory.h"
#include "../utils/Logger.h"

MessagesController::MessagesController(ConversationViewModel* viewModel, QObject* parent)
    : QObject(parent), m_viewModel(viewModel) {
    handlers = {
        {"SEND_MESSAGE_RESPONSE", [this](const QJsonObject& p) { handleSendMessageResponse(p); }},
        {"FETCH_MESSAGES_RESPONSE", [this](const QJsonObject& p) { handleFetchMessagesResponse(p); }}
    };

    connect(m_viewModel, &ConversationViewModel::sendMessageRequested, this, &MessagesController::sendMessage);
    connect(m_viewModel, &ConversationViewModel::fetchMessagesRequested, this, &MessagesController::fetchMessages);
}

bool MessagesController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

void MessagesController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        handlers[type](payload);
    }
}

void MessagesController::sendMessage(const QString& contact, const QString& content) {
    QByteArray requestData = RequestFactory::createSendMessageRequest(contact, content);
    emit sendRequest(requestData);
}

void MessagesController::fetchMessages(const QString& contact) {
    QByteArray requestData = RequestFactory::createFetchMessagesRequest(contact);
    emit sendRequest(requestData);
}

void MessagesController::handleSendMessageResponse(const QJsonObject& response) {
    bool success = response.value("success").toBool();
    if (success) {
        QString author = response.value("author").toString();
        QString content = response.value("content").toString();
        QDateTime timestamp = QDateTime::fromString(response.value("timestamp").toString(), Qt::ISODate);
        m_viewModel->onMessageReceived(author, content, timestamp);
    } else {
        Logger::error("Failed to send message");
    }
}

void MessagesController::handleFetchMessagesResponse(const QJsonObject& response) {
    QJsonArray messagesArray = response.value("messages").toArray();
    QVector<Message> messages;
    for (const QJsonValue& value : messagesArray) {
        QJsonObject obj = value.toObject();
        QString author = obj.value("author").toString();
        QString content = obj.value("content").toString();
        QDateTime timestamp = QDateTime::fromString(obj.value("timestamp").toString(), Qt::ISODate);
        messages.append({author, content, timestamp, author == m_viewModel->currentContact()});
    }
    m_viewModel->onMessagesFetched(messages);
}
