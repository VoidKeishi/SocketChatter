#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "controllers/ControllerManager.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Set Material style
    QQuickStyle::setStyle("Material");

    // Instantiate controllers & viewmodels
    auto manager = ControllerManager::instance();
    manager->initControllers();

    // Set up QML engine
    QQmlApplicationEngine engine;
    // Expose controllers and viewmodels to QML
    engine.rootContext()->setContextProperty("authController", manager->authController);
    engine.rootContext()->setContextProperty("contactsController", manager->contactsController);
    engine.rootContext()->setContextProperty("authViewModel", manager->authViewModel());
    engine.rootContext()->setContextProperty("contactViewModel", manager->contactViewModel());

    // Load initial QML (AuthWindow)
    engine.load(QUrl(QStringLiteral("qrc:/src/views/AuthWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}