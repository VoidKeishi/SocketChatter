#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QQmlEngine> 
#include "controllers/ControllerManager.h"
#include "controllers/utils/Logger.h"
#include "controllers/network/NetworkController.h"
#include "viewmodels/ConversationViewModel.h"

int main(int argc, char *argv[]) {
    qmlRegisterType<QmlMessage>("Message", 1, 0, "Message");
    qmlRegisterType<ConversationViewModel>("ConversationViewModel", 1, 0, "ConversationViewModel");
    
    QGuiApplication app(argc, argv);

    // Validate command line arguments
    if (argc < 3) {
        qDebug() << "Usage: ./Client <ip_address> <port> [log_level]";
        qDebug() << "Example: ./Client 192.168.1.100 3000 DEBUG";
        return -1;
    }

    // Extract server connection details
    QString serverIP = QString::fromUtf8(argv[1]);
    int serverPort = QString::fromUtf8(argv[2]).toInt();

    // Configure logging
    if (argc >= 4) {
        QString logLevelStr = QString::fromUtf8(argv[3]).toUpper();
        Logger::setLogLevel(logLevelStr);
    } else {
        Logger::setLogLevel(LogLevel::DEBUG);
    }

    // Connect to server using provided IP and port
    NetworkController::instance()->connectToServer(serverIP, serverPort);

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
    engine.rootContext()->setContextProperty("groupsController", manager->groupsController);
    engine.rootContext()->setContextProperty("authViewModel", manager->authViewModel());
    engine.rootContext()->setContextProperty("contactViewModel", manager->contactViewModel());
    engine.rootContext()->setContextProperty("conversationViewModel", manager->conversationViewModel());
    engine.rootContext()->setContextProperty("groupViewModel", manager->groupViewModel());
    
    // Load initial QML (AuthWindow)
    engine.load(QUrl(QStringLiteral("qrc:/src/views/AuthWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}