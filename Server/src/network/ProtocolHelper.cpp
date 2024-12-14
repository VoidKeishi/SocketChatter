#include "ProtocolHelper.h"
#include <QJsonDocument>

QJsonObject ProtocolHelper::createResponse(const QString& type, const QJsonObject& payload) {
    QJsonObject response;
    response["type"] = type;
    response["payload"] = payload;
    return response;
}

void ProtocolHelper::sendMessage(QTcpSocket* socket, const QJsonObject& message) {
    QByteArray data = serialize(message);
    socket->write(data);
}

QByteArray ProtocolHelper::serialize(const QJsonObject& json) {
    QJsonDocument doc(json);
    return doc.toJson(QJsonDocument::Compact);
}

QJsonObject ProtocolHelper::deserialize(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}