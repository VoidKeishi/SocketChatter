#include "ConversationViewModel.h"

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
    emit sendMessageRequested(sender, receiver, content);
}

void ConversationViewModel::fetchMessages() {
    emit fetchMessagesRequested(UserManager::instance()->m_currentUser, m_currentReceiver);
}

void ConversationViewModel::onMessageReceived(
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