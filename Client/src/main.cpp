#include <QGuiApplication>
#include "controllers/ControllerManager.h"
#include "views/WindowManager.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    auto controllerManager = ControllerManager::instance();
    auto windowManager = WindowManager::instance();
    
    windowManager->setControllerManager(controllerManager);
    windowManager->initializeQml();

    return app.exec();
}