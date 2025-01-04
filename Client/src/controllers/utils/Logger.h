#ifndef LOGGER_H
#define LOGGER_H

#include <QJsonObject>
#include <QString>
#include <QDateTime>
#include <QJsonDocument>
#include <QDebug>
#include <QRandomGenerator>
#include <QJsonArray>
#include <QJsonValue>
#include <cmath>
#include <atomic>

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    JSON = 2,
    ERROR = 3
};

enum class LogColor {
    RESET   = 0,
    RED     = 31,
    GREEN   = 32,
    YELLOW  = 33,
    BLUE    = 34,
    MAGENTA = 35,
    CYAN    = 36,
    WHITE   = 37,
    GRAY    = 90
};

class Logger {
public:
    // Set the current log level
    static void setLogLevel(LogLevel setLevel) {
        level.store(setLevel, std::memory_order_relaxed);
    }

    static void setLogLevel(const QString& setLevel) {
        if (setLevel == "DEBUG") {
            setLogLevel(LogLevel::DEBUG);
        }
        else if (setLevel == "INFO") {
            setLogLevel(LogLevel::INFO);
        }
        else if (setLevel == "JSON") {
            setLogLevel(LogLevel::JSON);
        }
        else if (setLevel == "ERROR") {
            setLogLevel(LogLevel::ERROR);
        }
        else {
            setLogLevel(LogLevel::DEBUG);
        }
    }

    // Logging methods
    static void error(const QString& message) {
        log(LogLevel::ERROR, message);
    }
    
    static void json(const QString& message, const QJsonObject& json) {
        log(LogLevel::JSON, message, json);
    }

    static void info(const QString& message) {
        log(LogLevel::INFO, message);
    }

    static void debug(const QString& message) {
        log(LogLevel::DEBUG, message);
    }

private:
    // Current log level (default: DEBUG)
    static std::atomic<LogLevel> level;

    // Colorize a given text with the specified color
    static QString colorize(const QString& text, LogColor color) {
        return QString("\033[%1m%2\033[0m").arg(static_cast<int>(color)).arg(text);
    }

    // Convert LogLevel to corresponding LogColor
    static LogColor levelToColor(LogLevel lvl) {
        switch (lvl) {
            case LogLevel::DEBUG: return LogColor::BLUE;
            case LogLevel::INFO: return LogColor::GREEN;
            case LogLevel::JSON: return LogColor::YELLOW;
            case LogLevel::ERROR: return LogColor::RED;
            default: return LogColor::WHITE;
        }
    }

    // Convert LogLevel to string
    static QString levelToString(LogLevel lvl) {
        switch (lvl) {
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::JSON: return "JSON";
            case LogLevel::INFO: return "INFO";
            case LogLevel::DEBUG: return "DEBUG";
            default: return "UNKNOWN";
        }
    }

    // Recursive function to colorize JSON
    static QString colorizeJson(const QJsonValue& value, int indentLevel = 0) {
        QString indent(indentLevel * 4, ' ');
        QString coloredString;

        if (value.isObject()) {
            coloredString += colorize("{\n", LogColor::WHITE);
            QJsonObject obj = value.toObject();
            int count = 0;
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                coloredString += indent + "    " + colorize(QString("\"%1\"").arg(it.key()), LogColor::GREEN) + ": ";
                coloredString += colorizeJson(it.value(), indentLevel + 1);
                if (count < obj.size() - 1)
                    coloredString += ",";
                coloredString += "\n";
                count++;
            }
            coloredString += indent + colorize("}", LogColor::WHITE);
        }
        else if (value.isArray()) {
            coloredString += colorize("[\n", LogColor::WHITE);
            QJsonArray arr = value.toArray();
            for (int i = 0; i < arr.size(); ++i) {
                coloredString += indent + "    " + colorizeJson(arr.at(i), indentLevel + 1);
                if (i < arr.size() - 1)
                    coloredString += ",";
                coloredString += "\n";
            }
            coloredString += indent + colorize("]", LogColor::WHITE);
        }
        else if (value.isString()) {
            coloredString += colorize(QString("\"%1\"").arg(value.toString()), LogColor::WHITE);
        }
        else if (value.isDouble()) {
            double num = value.toDouble();
            QString formattedNumber;

            if (std::floor(num) == num) {
                formattedNumber = QString::number(static_cast<long long>(num));
            }
            else {
                formattedNumber = QString::number(num, 'f', 2);
            }

            coloredString += colorize(formattedNumber, LogColor::WHITE);
        }
        else if (value.isBool()) {
            coloredString += colorize(value.toBool() ? "true" : "false", LogColor::WHITE);
        }
        else if (value.isNull()) {
            coloredString += colorize("null", LogColor::WHITE);
        }
        else {
            coloredString += colorize("UNKNOWN", LogColor::WHITE);
        }

        return coloredString;
    }

    // Convert JsonObject to colorized JSON string
    static QString colorizeJsonString(const QString& jsonStr) {
        QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
        if (!doc.isObject()) {
            return colorize("Invalid JSON", LogColor::RED);
        }
        return colorizeJson(doc.object());
    }

    // Log method for non-JSON messages
    static void log(LogLevel msgLevel, const QString& message) {
        if (static_cast<int>(msgLevel) < static_cast<int>(level.load(std::memory_order_relaxed))) {
            return; // Do not log messages below the current log level
        }

        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        QString coloredTimestamp = colorize(timestamp, LogColor::GRAY);
        QString coloredLevel = colorize(levelToString(msgLevel), levelToColor(msgLevel));
        QString coloredMessage = colorize(message, LogColor::WHITE);

        qDebug().noquote() << QString("[%1] [%2] %3")
            .arg(coloredTimestamp)
            .arg(coloredLevel)
            .arg(coloredMessage);
    }

    // Overloaded log method for JSON messages
    static void log(LogLevel msgLevel, const QString& message, const QJsonObject& jsonContent) {
        if (static_cast<int>(msgLevel) < static_cast<int>(level.load(std::memory_order_relaxed))) {
            return; // Do not log messages below the current log level
        }

        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        QString coloredTimestamp = colorize(timestamp, LogColor::GRAY);
        QString coloredLevel = colorize(levelToString(msgLevel), levelToColor(msgLevel));
        QString coloredMessage = colorize(message + ":", LogColor::WHITE);
        QString coloredJson = colorizeJsonString(QJsonDocument(jsonContent).toJson(QJsonDocument::Indented));

        qDebug().noquote() << QString("[%1] [%2] %3\n%4")
            .arg(coloredTimestamp)
            .arg(coloredLevel)
            .arg(coloredMessage)
            .arg(coloredJson);
    }
};

#endif // LOGGER_H