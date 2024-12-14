#ifndef MESSAGESERIALIZER_H
#define MESSAGESERIALIZER_H

#include <QJsonObject>
#include <QByteArray>

class MessageSerializer {
public:
    static QByteArray serialize(const QJsonObject &message);
    static QJsonObject deserialize(const QByteArray &data, QString &errorString);
};

#endif // MESSAGESERIALIZER_H