#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include "RequestDispatcher.h"
#include "../handlers/AuthHandler.h"
#include "../handlers/ContactHandler.h"

class ClientHandler : public QObject {
    Q_OBJECT

public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);
    void setUsername(const QString& user);

signals:
    void disconnected();
    void finished();

public slots:
    void start();
    void cleanup();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* clientSocket;
    QByteArray buffer;
    RequestDispatcher* dispatcher;
    AuthHandler* authHandler;
    ContactHandler* contactHandler;
    QString username;
    void processRequest(const QJsonObject& request);
    void sendResponse(const QJsonObject& response);
};

#endif // CLIENTHANDLER_H