#include <QJsonDocument>

#include "NetworkController.h"
#include "../utils/Logger.h"

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
        Logger::debug("Already connected or connecting");
    }
}

void NetworkController::onConnected() {
    Logger::debug("Connected to server");
    emit connected();
}

void NetworkController::sendData(const QByteArray &data) {
    Logger::json("Sending data", QJsonDocument::fromJson(data).object());
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray framedData = data + '\n';
        qint64 bytesWritten = socket->write(framedData);
        if (bytesWritten == -1) {
            Logger::error(QString("Failed to write to socket. Error: %1")
                .arg(socket->errorString()));
            return;
        } else 
            Logger::info(QString("Successfully wrote %1 bytes").arg(bytesWritten));
            
        socket->flush();
    } else {
        Logger::error("Socket is not connected");
    }
}

void NetworkController::onReadyRead() {
    buffer.append(socket->readAll());
    int startIndex = 0;

    while ((startIndex = buffer.indexOf('{', startIndex)) != -1) {
        int endIndex = buffer.indexOf('\n', startIndex);
        if (endIndex == -1) break;  // No complete line yet

        QByteArray jsonData = buffer.mid(startIndex, endIndex - startIndex);
        
        // Validate JSON
        QJsonParseError error;
        QJsonDocument::fromJson(jsonData, &error);
        
        if (error.error == QJsonParseError::NoError) {
            emit rawDataReceived(jsonData);
            buffer.remove(0, endIndex + 1);
            startIndex = 0;  // Reset for next search
        } else {
            startIndex = endIndex + 1;  // Try next JSON object
        }
    }
}

void NetworkController::onDisconnected() {
    Logger::debug("Disconnected from server");
    emit disconnected();
}

void NetworkController::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    Logger::error("Socket error: " + socket->errorString());
}