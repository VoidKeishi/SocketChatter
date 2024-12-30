#ifndef LOGGER_H
#define LOGGER_H

#include <QJsonObject>
#include <QString>
#include <QDateTime>
#include <QJsonDocument>
#include <QDebug>

class Logger {
public:
    static void debug(const QString& message) {
        log("DEBUG", message);
    }
    
    static void info(const QString& message) {
        log("INFO", message);
    }
    
    static void json(const QString& prefix, const QJsonObject& json) {
        // Manually construct the JSON string in the desired order
        QString formatted = "{\n";
        formatted += QString("    \"type\": \"%1\",\n").arg(json["type"].toString());
        formatted += QString("    \"timestamp\": %1,\n").arg(json["timestamp"].toInt());

        QJsonObject payload = json["payload"].toObject();
        QString payloadStr = "{\n";
        
        // Ensure the payload fields are ordered as needed
        QStringList payloadKeys = payload.keys();
        for (int i = 0; i < payloadKeys.size(); ++i) {
            QString key = payloadKeys.at(i);
            QString value;
            if (payload[key].isString()) {
                value = QString("\"%1\"").arg(payload[key].toString());
            } else if (payload[key].isDouble()) {
                value = QString::number(payload[key].toDouble());
            } else if (payload[key].isBool()) {
                value = payload[key].toBool() ? "true" : "false";
            } else {
                value = payload[key].toString();
            }

            // Add comma except for the last item
            QString comma = (i < payloadKeys.size() - 1) ? "," : "";
            payloadStr += QString("        \"%1\": %2%3\n").arg(key, value, comma);
        }
        payloadStr += "    }";

        formatted += QString("    \"payload\": %1\n").arg(payloadStr);
        formatted += "}";

        log("JSON", prefix + ":\n" + formatted);
    }

private:
    static void log(const QString& level, const QString& message) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        qDebug().noquote() << QString("[%1] [%2] %3").arg(timestamp, level, message);
    }
};

#endif // LOGGER_H