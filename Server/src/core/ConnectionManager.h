#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include "ClientHandler.h"

class ConnectionManager : public QObject {
    Q_OBJECT
public:
    static ConnectionManager* instance();

    void addClient(const QString& username, ClientHandler* handler);
    void removeClient(const QString& username);
    ClientHandler* getClientHandler(const QString& username) const;

    void handleNewConnection(QTcpSocket* clientSocket);

    bool sendMessageToClient(const QString& toUsername, const QJsonObject& message);

private:
    explicit ConnectionManager(QObject* parent = nullptr);
    static ConnectionManager* m_instance;

    QMap<QString, ClientHandler*> m_clients;
};
