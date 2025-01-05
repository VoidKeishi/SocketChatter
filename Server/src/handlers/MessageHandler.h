#pragma once
#include "BaseHandler.h"
#include <QMap>
#include <QTcpSocket>

class MessageHandler : public BaseHandler {
    Q_OBJECT
public:
    explicit MessageHandler(QObject* parent = nullptr);
    
    void registerClient(const QString& username, QTcpSocket* socket);
    void removeClient(const QString& username);

private:
    QMap<QString, QTcpSocket*> activeClients;
};