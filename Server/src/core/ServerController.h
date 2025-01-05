#pragma once

#include <QTcpServer>
#include <QObject>
#include <QString>

class ServerController : public QObject {
    Q_OBJECT

public:
    explicit ServerController(QObject* parent = nullptr);
    void startServer(quint16 port);

private slots:
    void onNewConnection();

private:
    QTcpServer* serverSocket;
};