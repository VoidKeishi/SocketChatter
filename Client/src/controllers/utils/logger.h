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
        QJsonDocument doc(json);
        QString formatted = doc.toJson(QJsonDocument::Indented);
        log("JSON", prefix + ":\n" + formatted);
    }

private:
    static void log(const QString& level, const QString& message) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        qDebug().noquote() << QString("[%1] [%2] %3").arg(timestamp, level, message);
    }
};

#endif // LOGGER_H