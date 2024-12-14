#include "MessageSerializer.h"
#include <QJsonDocument>

QByteArray MessageSerializer::serialize(const QJsonObject &message) {
    return QJsonDocument(message).toJson(QJsonDocument::Compact) + "\n";
}

QJsonObject MessageSerializer::deserialize(const QByteArray &data, QString &errorString) {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        errorString = parseError.errorString();
        return QJsonObject();
    }
    errorString.clear();
    return doc.object();
}