#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>

struct Message {
    QString author;
    QString content;
    QDateTime timestamp;
    bool sentByMe;
};

class ConversationViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentContact READ currentContact WRITE setCurrentContact NOTIFY currentContactChanged)
    Q_PROPERTY(QVector<Message> messages READ messages NOTIFY messagesChanged)

public:
    explicit ConversationViewModel(QObject* parent = nullptr);

    QString currentContact() const { return m_currentContact; }
    void setCurrentContact(const QString& contact);

    QVector<Message> messages() const { return m_messages; }

    Q_INVOKABLE void sendMessage(const QString& content);
    Q_INVOKABLE void fetchMessages();

signals:
    void currentContactChanged();
    void messagesChanged();
    void sendMessageRequested(const QString& contact, const QString& content);
    void fetchMessagesRequested(const QString& contact);

public slots:
    void onMessageReceived(const QString& author, const QString& content, const QDateTime& timestamp);
    void onMessagesFetched(const QVector<Message>& messages);

private:
    QString m_currentContact;
    QVector<Message> m_messages;
};