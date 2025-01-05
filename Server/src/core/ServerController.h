#pragma once

#include <QTcpServer>
#include <QObject>
#include <QMap>
#include <QString>

class ClientHandler; // Forward declaration

class ServerController : public QObject {
    Q_OBJECT

public:
    explicit ServerController(QObject* parent = nullptr);
    void startServer(quint16 port);

    // Methods to manage clients
    void addClient(const QString& username, ClientHandler* client);
    void removeClient(const QString& username);
    ClientHandler* getClientHandler(const QString& username) const;

signals:
    void clientAdded(const QString& username);
    void clientRemoved(const QString& username);

private slots:
    void onNewConnection();

private:
    QTcpServer* serverSocket;
    QMap<QString, ClientHandler*> clients;
};
