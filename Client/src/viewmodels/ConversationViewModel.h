#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QAbstractListModel>
#include <QVariant>
#include <QDebug>

struct Message {
    QString author;
    QString content;
    QDateTime timestamp;
    bool sentByMe; 
};

class QmlMessage : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString author READ author CONSTANT)
    Q_PROPERTY(QString content READ content CONSTANT)
    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT)
    Q_PROPERTY(bool sentByMe READ sentByMe CONSTANT)

public:
    explicit QmlMessage(const Message& msg, QObject* parent = nullptr) 
        : QObject(parent), m_msg(msg) {}

    QString author() const { return m_msg.author; }
    QString content() const { return m_msg.content; }
    QDateTime timestamp() const { return m_msg.timestamp; }
    bool sentByMe() const { return m_msg.sentByMe; }

private:
    Message m_msg;
};

class ConversationViewModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString currentContact READ currentContact WRITE setCurrentContact NOTIFY currentContactChanged)
    // Q_PROPERTY(QVector<Message> messages READ messages NOTIFY messagesChanged)

public:
    enum Roles {
        AuthorRole = Qt::UserRole+1, 
        ContentRole, 
        TimestampRole, 
        SentByMeRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return m_messages.size(); 
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_messages.size()) return QVariant(); 

        const Message &msg = m_messages[index.row()];
        switch (role) {
            case AuthorRole: return msg.author; 
            case ContentRole: return msg.content; 
            case TimestampRole: return msg.timestamp; 
            case SentByMeRole: return msg.sentByMe; 
            default: return QVariant(); 
        }
    }

    QHash<int, QByteArray> roleNames() const override {
        return {
            {AuthorRole, "author"},
            {ContentRole, "content"},
            {TimestampRole, "timestamp"},
            {SentByMeRole, "sentByMe"}
        };
    }

    explicit ConversationViewModel(QObject* parent = nullptr);

    QString currentContact() const { return m_currentContact; }
    // void setCurrentContact(const QString& contact);

    // QVector<Message> messages() const { return m_messages; }

    Q_INVOKABLE void sendMessage(const QString& content);
    Q_INVOKABLE void fetchMessages();

signals:
    void currentContactChanged();
    // void messagesChanged();
    void sendMessageRequested(const QString& contact, const QString& content);
    void fetchMessagesRequested(const QString& contact);

public slots:
    void setCurrentContact(const QString& contact);
    void onMessageReceived(const QString& author, const QString& content, const QDateTime& timestamp);
    void onMessagesFetched(const QVector<Message>& messages);

private:
    QString m_currentContact;
    QVector<Message> m_messages;
};