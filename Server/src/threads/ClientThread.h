#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "../controllers/ClientHandler.h"

class ClientThread : public QThread {
    Q_OBJECT

public:
    explicit ClientThread(QTcpSocket* socket, QObject* parent = nullptr);
    void run() override;

private:
    QTcpSocket* clientSocket;
};

#endif // CLIENTTHREAD_H