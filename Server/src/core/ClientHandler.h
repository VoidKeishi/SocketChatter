#pragma once

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
    void sendResponse(const QJsonObject& response);

signals:
    void disconnected();
    void finished();

public slots:
    void start();
    void cleanup();

private slots:
    void onReadyRead();
    void onDisconnected();
    void onLoginSuccess(const QString& username);

private:
    QTcpSocket* clientSocket;
    QByteArray buffer;
    RequestDispatcher* dispatcher;
    AuthHandler* authHandler;
    ContactHandler* contactHandler;
    QString m_username;
    void processRequest(const QJsonObject& request);
};
