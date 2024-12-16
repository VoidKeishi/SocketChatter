#include "NetworkController.h"
#include <QDebug>

NetworkController* NetworkController::m_instance = nullptr;

NetworkController* NetworkController::instance() {
    if (!m_instance) {
        m_instance = new NetworkController();
    }
    return m_instance;
}

NetworkController::NetworkController(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &NetworkController::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkController::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkController::onDisconnected);
    connect(socket, &QAbstractSocket::errorOccurred, this, &NetworkController::onErrorOccurred);
}

void NetworkController::connectToServer(const QString &host, quint16 port) {
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost(host, port);
    } else {
        qDebug() << "Already connected or connecting";
    }
}

void NetworkController::sendData(const QByteArray &data) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(data);
        socket->flush();
    } else {
        qWarning() << "Socket is not connected";
    }
}

void NetworkController::onConnected() {
    qDebug() << "Connected to server";
    emit connected();
}

void NetworkController::onReadyRead() {
    buffer.append(socket->readAll());
    int index;
    while ((index = buffer.indexOf('\n')) != -1) {
        QByteArray data = buffer.left(index);
        buffer.remove(0, index + 1);
        emit dataReceived(data);
    }
}

void NetworkController::onDisconnected() {
    qDebug() << "Disconnected from server";
    emit disconnected();
}

void NetworkController::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qWarning() << "Socket error:" << socket->errorString();
}