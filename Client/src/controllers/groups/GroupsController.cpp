#include "MessagesController.h"
#include "MessagesRequestSender.h"
#include "MessagesResponseHandler.h"
#include "MessagesNotificationHandler.h"

MessagesController::MessagesController(ConversationViewModel* viewModel, QObject* parent)
    : QObject(parent) 
    , m_viewModel(viewModel) 
    , m_requestSender(new MessagesRequestSender(this))
    , m_responseHandler(new MessagesResponseHandler(viewModel, this))
    , m_notificationHandler(new MessagesNotificationHandler(viewModel, this))
{
    handlers = {
        // Responses
        {"SEND_MESSAGE_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleSendMessageResponse(p); }},
        {"FETCH_MESSAGES_RESPONSE", [this](const QJsonObject& p) { m_responseHandler->handleFetchMessagesResponse(p); }},
        // Notifications
        {"MESSAGE_NOTIFICATION", [this](const QJsonObject& p) { m_notificationHandler->handleMessageNotification(p); }},
    };

    connect(m_viewModel, &ConversationViewModel::messageActionRequested, 
            this, &MessagesController::handleMessageAction);
    connect(this, &MessagesController::sendRequest, 
            NetworkController::instance(), &NetworkController::sendData);
}

bool MessagesController::canHandle(const QString& type) const {
    return handlers.contains(type);
}

// Handle responses and notifications
void MessagesController::handle(const QString& type, const QJsonObject& payload) {
    if (handlers.contains(type)) {
        handlers[type](payload);
    }
}
// Handle UI actions
void MessagesController::handleMessageAction(MessageAction action, const QString& sender, const QString& receiver, const QString& content) {
    switch (action) {
    case MessageAction::SendMessage:
        Logger::debug("Sending message");
        m_requestSender->requestSendMessage(sender, receiver, content);
        break;
    case MessageAction::FetchMessages:
        m_requestSender->requestFetchMessages(sender, receiver);
        break;
    default:
        Logger::error("Invalid message action");
        break;
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