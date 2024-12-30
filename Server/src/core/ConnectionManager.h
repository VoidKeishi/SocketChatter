#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QMap>
#include "ClientHandler.h"

class ConnectionManager : public QObject {
    Q_OBJECT
public:
    static ConnectionManager* instance();

    void addClient(const QString& username, ClientHandler* handler);
    void removeClient(const QString& username);
    ClientHandler* getClientHandler(const QString& username) const;

private:
    explicit ConnectionManager(QObject* parent = nullptr);
    static ConnectionManager* m_instance;

    QMap<QString, ClientHandler*> m_clients;
};

#endif // CONNECTIONMANAGER_H