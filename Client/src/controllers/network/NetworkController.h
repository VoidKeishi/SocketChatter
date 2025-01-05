#pragma once

#include <QObject>
#include <QTcpSocket>

class NetworkController : public QObject {
    Q_OBJECT
public:
    static NetworkController* instance();

    void connectToServer(const QString &host, quint16 port);

public slots:
    void sendData(const QByteArray &data);    

signals:
    void rawDataReceived(const QByteArray &data);
    void connected();
    void disconnected();

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    explicit NetworkController(QObject *parent = nullptr);
    static NetworkController* m_instance;

    QTcpSocket *socket;
    QByteArray buffer;
};
