#include "Logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

void Logger::log(const QString& message) {
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QFile file("server.log");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString(Qt::ISODate) << " - " << message << "\n";
        file.close();
    }
}