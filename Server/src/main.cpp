#include <QCoreApplication>
#include "controllers/ServerController.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    ServerController serverController;
    serverController.startServer(3000);

    return app.exec();
}