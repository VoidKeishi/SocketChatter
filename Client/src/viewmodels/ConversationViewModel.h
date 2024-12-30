#pragma once
#include <QAbstractListModel>
#include <QDateTime>

class ConversationViewModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)

public:
    struct Message {
        QString author;
        QString content;
        QDateTime timestamp;
        bool sentByMe;
    };

    enum MessageRoles {
        AuthorRole = Qt::UserRole + 1,
        ContentRole,
        TimestampRole,
        SentByMeRole
    };

    explicit ConversationViewModel(QObject* parent = nullptr);
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString recipient() const { return m_recipient; }
    void setRecipient(const QString& recipient);

    Q_INVOKABLE void sendMessage(const QString& content);

signals:
    void recipientChanged();

private:
    QString m_recipient;
    QVector<Message> m_messages;
    void addTestData();
};