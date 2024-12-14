#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger {
public:
    static void log(const QString& message);
};

#endif // LOGGER_H