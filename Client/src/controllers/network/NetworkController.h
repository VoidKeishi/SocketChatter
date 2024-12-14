#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include <QObject>
#include <QTcpSocket>

class NetworkController : public QObject {
    Q_OBJECT
public:
    static NetworkController* instance();

    void connectToServer(const QString &host, quint16 port);
    void sendData(const QByteArray &data);

signals:
    void dataReceived(const QByteArray &data);
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

#endif // NETWORKCONTROLLER_H