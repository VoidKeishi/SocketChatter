#include "ConversationViewModel.h"
#include "../controllers/utils/Logger.h"

ConversationViewModel::ConversationViewModel(QObject* parent)
    : QAbstractListModel(parent) {}

void ConversationViewModel::setCurrentReceiver(const QString& receiver) {
    if (m_currentReceiver != receiver) {
        m_currentReceiver = receiver;
        emit currentReceiverChanged();
        fetchMessages();
    }
}

void ConversationViewModel::sendMessage(const QString& sender, const QString& receiver, const QString& content) {
    emit messageActionRequested(MessageAction::SendMessage, sender, receiver, content);
}

void ConversationViewModel::fetchMessages() {
    emit messageActionRequested(MessageAction::FetchMessages, 
                              UserManager::instance()->currentUser(), 
                              m_currentReceiver);
}

void ConversationViewModel::onMessageAckReceived(
    const QString& sender,
    const QString& receiver,
    const QString& content,
    const QDateTime& timestamp,
    const QString& id) 
{
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    m_messages.append({
        id.isEmpty() ? QString::number(QDateTime::currentMSecsSinceEpoch()) : id,
        sender,
        receiver,
        content,
        timestamp
    });
    endInsertRows();
}

void ConversationViewModel::onMessagesFetched(const QVector<Message>& messages) {
    beginResetModel();
    m_messages = messages;
    endResetModel();
}

// Updater for controller to call
void ConversationViewModel::addMessage(const QString &from, const QString &content) {
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    m_messages.append({
        QString::number(QDateTime::currentMSecsSinceEpoch()),
        from,
        UserManager::instance()->currentUser(),
        content,
        QDateTime::currentDateTime()
    });
    endInsertRows();
}