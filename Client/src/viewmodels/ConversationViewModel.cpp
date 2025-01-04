#include "ConversationViewModel.h"

ConversationViewModel::ConversationViewModel(QObject* parent)
    : QObject(parent) {}

void ConversationViewModel::setCurrentContact(const QString& contact) {
    if (m_currentContact != contact) {
        m_currentContact = contact;
        emit currentContactChanged();
        fetchMessages();
    }
}

void ConversationViewModel::sendMessage(const QString& content) {
    emit sendMessageRequested(m_currentContact, content);
}

void ConversationViewModel::fetchMessages() {
    emit fetchMessagesRequested(m_currentContact);
}

void ConversationViewModel::onMessageReceived(const QString& author, const QString& content, const QDateTime& timestamp) {
    m_messages.append({author, content, timestamp, author == m_currentContact});
    emit messagesChanged();
}

void ConversationViewModel::onMessagesFetched(const QVector<Message>& messages) {
    m_messages = messages;
    emit messagesChanged();
}
