#include <QCoreApplication>

#include "core/ServerController.h"
#include "utils/Logger.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    if (argc >= 1) {
        QString logLevelStr = QString::fromUtf8(argv[1]).toUpper();
        Logger::setLogLevel(logLevelStr);
    }
    else {
        Logger::setLogLevel(LogLevel::DEBUG);
    }

    ServerController serverController;
    serverController.startServer(3000);

    return app.exec();
}