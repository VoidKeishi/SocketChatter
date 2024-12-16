#ifndef PROTOCOLHELPER_H
#define PROTOCOLHELPER_H

#include <QJsonObject>
#include <QTcpSocket>

class ProtocolHelper {
public:
    static QJsonObject createResponse(const QString& type, const QJsonObject& payload);
    static void sendMessage(QTcpSocket* socket, const QJsonObject& message);
    static QByteArray serialize(const QJsonObject& json);
    static QJsonObject deserialize(const QByteArray& data);
};

#endif // PROTOCOLHELPER_H