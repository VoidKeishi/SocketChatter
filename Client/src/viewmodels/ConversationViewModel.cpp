#include "ConversationViewModel.h"

ConversationViewModel::ConversationViewModel(QObject* parent)
    : QAbstractListModel(parent)
{
    addTestData();
}

int ConversationViewModel::rowCount(const QModelIndex& parent) const
{
    return m_messages.count();
}

QVariant ConversationViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Message& message = m_messages[index.row()];

    switch (role) {
        case AuthorRole:
            return message.author;
        case ContentRole:
            return message.content;
        case TimestampRole:
            return message.timestamp;
        case SentByMeRole:
            return message.sentByMe;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> ConversationViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AuthorRole] = "author";
    roles[ContentRole] = "content";
    roles[TimestampRole] = "timestamp";
    roles[SentByMeRole] = "sentByMe";
    return roles;
}

void ConversationViewModel::setRecipient(const QString& recipient)
{
    if (m_recipient != recipient) {
        m_recipient = recipient;
        emit recipientChanged();
    }
}

void ConversationViewModel::sendMessage(const QString& content)
{
    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    m_messages.append({
        "Me",
        content,
        QDateTime::currentDateTime(),
        true
    });
    endInsertRows();
}

void ConversationViewModel::addTestData()
{
    m_messages = {
        {"John", "Hey there!", QDateTime::currentDateTime().addSecs(-3600), false},
        {"Me", "Hi John!", QDateTime::currentDateTime().addSecs(-3500), true},
        {"John", "How are you?", QDateTime::currentDateTime().addSecs(-3400), false},
        {"Me", "I'm good, thanks!", QDateTime::currentDateTime().addSecs(-3300), true}
    };
}