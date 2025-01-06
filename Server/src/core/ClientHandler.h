#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

#include "RequestDispatcher.h"
#include "../handlers/AuthHandler.h"
#include "../handlers/ContactHandler.h"
#include "../handlers/MessageHandler.h"
#include "../handlers/GroupHandler.h"

class ClientHandler : public QObject {
    Q_OBJECT

public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);
    void sendResponse(const QJsonObject& response);

signals:
    void finished();

public slots:
    void start();
    void cleanup();
    void onLoginSuccess(const QString& username);
    void handleMessage(const QString& toUsername, const QJsonObject& message);

private slots:
    void onReadyRead();
    void onDisconnected();


private:
    QTcpSocket* clientSocket;
    QByteArray buffer;

    RequestDispatcher* dispatcher;
    AuthHandler* authHandler;
    ContactHandler* contactHandler;
    MessageHandler* messageHandler;
    GroupHandler* groupHandler;

    QString m_username;
    void processRequest(const QJsonObject& request);
};
