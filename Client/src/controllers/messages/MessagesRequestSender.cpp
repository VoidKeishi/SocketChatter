#include "MessagesRequestSender.h"
#include "../utils/RequestFactory.h"
#include "../network/NetworkController.h"

MessagesRequestSender::MessagesRequestSender(QObject* parent) 
    : QObject(parent)
{
    connect(this, &MessagesRequestSender::sendRequest,
            NetworkController::instance(), &NetworkController::sendData);
}

void MessagesRequestSender::requestSendMessage(const QString& sender, const QString& receiver, const QString& content) {
    emit sendRequest(RequestFactory::createSendMessageRequest(sender, receiver, content));
}

void MessagesRequestSender::requestFetchMessages(const QString& sender, const QString& receiver) {
    emit sendRequest(RequestFactory::createFetchMessagesRequest(sender, receiver));
}