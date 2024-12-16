#ifndef CONVERSATIONMODEL_H
#define CONVERSATIONMODEL_H
#include <QQmlEngine>
#include <QSqlTableModel>

class SqlConversationModel : public QSqlTableModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)

public:
    explicit SqlConversationModel(QObject *parent = nullptr);

    QString recipient() const;
    void setRecipient(const QString &recipient);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void sendMessage(const QString &recipient, const QString &message);

signals:
    void recipientChanged();

private:
    QString m_recipient;
};
#endif // CONVERSATIONMODEL_H