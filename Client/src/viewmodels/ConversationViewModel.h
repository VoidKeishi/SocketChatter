#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QAbstractListModel>
#include <QVariant>
#include <QDebug>
#include "../controllers/session/UserManager.h" 

struct Message {
    QString id;
    QString sender;
    QString receiver;
    QString content;
    QDateTime timestamp;

    bool isSentBy(const QString& userId) const {
        return sender == userId;
    }
};

class QmlMessage : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString sender READ sender CONSTANT)
    Q_PROPERTY(QString receiver READ receiver CONSTANT)
    Q_PROPERTY(QString content READ content CONSTANT)
    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT)
    Q_PROPERTY(bool sentByMe READ sentByMe CONSTANT)

public:
    explicit QmlMessage(const Message& msg, QObject* parent = nullptr) 
        : QObject(parent), m_msg(msg) {}

    QString sender() const { return m_msg.sender; }
    QString receiver() const { return m_msg.receiver; }
    QString content() const { return m_msg.content; }
    QDateTime timestamp() const { return m_msg.timestamp; }
    bool sentByMe() const { 
        return m_msg.isSentBy(UserManager::instance()->m_currentUser);
    }

private:
    Message m_msg;
};

class ConversationViewModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString currentReceiver READ currentReceiver 
               WRITE setCurrentReceiver NOTIFY currentReceiverChanged)

public:
    enum Roles {
        SenderRole = Qt::UserRole + 1,
        ReceiverRole,
        ContentRole,
        TimestampRole,
        SentByMeRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return m_messages.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_messages.size()) 
            return QVariant();

        const Message &msg = m_messages[index.row()];
        switch (role) {
            case SenderRole: return msg.sender;
            case ReceiverRole: return msg.receiver;
            case ContentRole: return msg.content;
            case TimestampRole: return msg.timestamp;
            case SentByMeRole: return msg.isSentBy(UserManager::instance()->m_currentUser);
            default: return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const override {
        return {
            {SenderRole, "sender"},
            {ReceiverRole, "receiver"},
            {ContentRole, "content"},
            {TimestampRole, "timestamp"},
            {SentByMeRole, "sentByMe"}
        };
    }

    explicit ConversationViewModel(QObject* parent = nullptr);

    QString currentReceiver() const { return m_currentReceiver; }

    Q_INVOKABLE void sendMessage(const QString &sender, const QString &receiver, const QString &content);
    Q_INVOKABLE void fetchMessages();
    Q_INVOKABLE QString getCurrentUser() {
        return UserManager::instance()->m_currentUser;
    }

signals:
    void currentReceiverChanged();
    void sendMessageRequested(const QString& sender, const QString& receiver, const QString& content);
    void fetchMessagesRequested(const QString& sender, const QString& receiver);

public slots:
    void setCurrentReceiver(const QString& receiver);
    void onMessageReceived(
        const QString& sender, 
        const QString& receiver,
        const QString& content, 
        const QDateTime& timestamp,
        const QString& id = QString()
    );
    void onMessagesFetched(const QVector<Message>& messages);

private:
    QString m_currentReceiver;
    QVector<Message> m_messages;
};