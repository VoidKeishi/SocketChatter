#pragma once
#include "BaseHandler.h"
#include "../repositories/MessageRepository.h"
#include <QMap>
#include <QTcpSocket>

class MessageHandler : public BaseHandler {
    Q_OBJECT

public:
    explicit MessageHandler(DatabaseManager* db);
    void registerClient(const QString& username, QTcpSocket* socket);
    void removeClient(const QString& username);

private:
    QMap<QString, QTcpSocket*> activeClients;
    MessageRepository messageRepo;  // Add this

    void handleSendMessage(const QJsonObject& request);
    void handleFetchMessages(const QJsonObject& request);
    void attemptDelivery(const QString& to, const QJsonObject& message);
};