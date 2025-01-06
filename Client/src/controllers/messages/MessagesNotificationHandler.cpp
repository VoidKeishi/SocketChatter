#include <QJsonArray>

#include "MessagesNotificationHandler.h"
#include "../session/UserManager.h"
#include "../utils/Logger.h"


MessagesNotificationHandler::MessagesNotificationHandler(ConversationViewModel* viewModel, QObject* parent)
    : QObject(parent)
    , m_viewModel(viewModel)
{
}

void MessagesNotificationHandler::handleMessageNotification(const QJsonObject& notification){
    QString to = notification.value("to").toString();
    if (to != UserManager::instance()->currentUser()) {
        Logger::error("Received message for another user: " + to);
        return;
    }
    QString from = notification.value("from").toString();
    QString content = notification.value("content").toString();
    m_viewModel->addMessage(from, content);
    Logger::debug("Received message from: " + notification.value("from").toString());
}