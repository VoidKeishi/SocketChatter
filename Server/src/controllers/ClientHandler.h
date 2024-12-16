#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

class ClientHandler : public QObject {
    Q_OBJECT

public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = nullptr);

signals:
    void disconnected();

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* clientSocket;
    QByteArray buffer;

    void processRequest(const QJsonObject& request);
    void handleLoginRequest(const QJsonObject& payload);
    void handleRegisterRequest(const QJsonObject& payload);
    void handleMessageSend(const QJsonObject& payload);
    void handleFriendRequest(const QJsonObject& payload);
    void handleFetchPendingInvitations(const QJsonObject& payload);
    void handleFriendRequestResponse(const QJsonObject& payload);
    void handleFetchContactList(const QJsonObject& payload);
    void sendResponse(const QJsonObject& response);
    void notifyContactListUpdate(const QString& username);
    void sendResponseToSocket(QTcpSocket* socket, const QJsonObject& response);
};

#endif // CLIENTHANDLER_H