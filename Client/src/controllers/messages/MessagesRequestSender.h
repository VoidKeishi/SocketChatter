#pragma once

#include <QObject>
#include <QString>

class MessagesRequestSender : public QObject {
    Q_OBJECT

public:
    explicit MessagesRequestSender(QObject* parent = nullptr);

    void requestSendMessage(const QString& sender, const QString& receiver, const QString& content);
    void requestFetchMessages(const QString& sender, const QString& receiver);

signals:
    void sendRequest(const QByteArray& data);
};