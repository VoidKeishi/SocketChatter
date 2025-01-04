#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "controllers/ControllerManager.h"
#include "controllers/utils/Logger.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Configure log level based on command-line arguments
    if (argc >= 4) {
        QString logLevelStr = QString::fromUtf8(argv[3]).toUpper();
        Logger::setLogLevel(logLevelStr);
    }
    else {
        Logger::setLogLevel(LogLevel::DEBUG);
    }

    // Set Material style
    QQuickStyle::setStyle("Material");

    // Instantiate controllers & viewmodels
    auto manager = ControllerManager::instance();

    // Set up QML engine
    QQmlApplicationEngine engine;
    // Expose controllers and viewmodels to QML
    engine.rootContext()->setContextProperty("authController", manager->authController);
    engine.rootContext()->setContextProperty("contactsController", manager->contactsController);
    engine.rootContext()->setContextProperty("messagesController", manager->messagesController);
    engine.rootContext()->setContextProperty("authViewModel", manager->authViewModel());
    engine.rootContext()->setContextProperty("contactViewModel", manager->contactViewModel());
    engine.rootContext()->setContextProperty("conversationViewModel", manager->conversationViewModel());

    // Load initial QML (AuthWindow)
    engine.load(QUrl(QStringLiteral("qrc:/src/views/AuthWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}