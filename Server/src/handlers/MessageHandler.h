#pragma once
#include "BaseHandler.h"
#include "../repositories/MessageRepository.h"
#include <QMap>
#include <QTcpSocket>

class MessageHandler : public BaseHandler {
    Q_OBJECT

public:
    explicit MessageHandler(DatabaseManager* db);

private:
    QMap<QString, QTcpSocket*> activeClients;
    MessageRepository messageRepo;

    void handleSendMessage(const QJsonObject& request);
    void handleFetchMessages(const QJsonObject& request);
};