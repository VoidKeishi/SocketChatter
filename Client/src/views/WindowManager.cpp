#include "WindowManager.h"
#include <QQmlContext>
#include <QDebug>

WindowManager* WindowManager::m_instance = nullptr;

WindowManager* WindowManager::instance() {
    if (!m_instance) {
        m_instance = new WindowManager(nullptr);
    }
    return m_instance;
}

WindowManager::WindowManager(QObject* parent)
    : QObject(parent)
    , controllerManager(nullptr)
{
    qDebug() << "WindowManager created";
}

void WindowManager::setControllerManager(ControllerManager* manager) {
    controllerManager = manager;
}

void WindowManager::initializeQml() {
    if (!controllerManager) {
        qWarning() << "ControllerManager is not set";
        return;
    }

    registerTypes();
    registerModels();
    setupConnections();

    engine.load(QUrl(QStringLiteral("qrc:/src/views/AuthWindow.qml")));

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML";
        return;
    }

    emit initialized();
}

void WindowManager::registerModels() {
    auto context = engine.rootContext();
    
    // Controllers
    context->setContextProperty("authController", controllerManager->authController);
    context->setContextProperty("contactsController", controllerManager->contactsController);
    context->setContextProperty("windowManager", this);
    
    // Models
    context->setContextProperty("friendsModel", controllerManager->friendListModel);
    context->setContextProperty("sentRequestModel", controllerManager->sentRequestModel);
    context->setContextProperty("receivedRequestModel", controllerManager->receivedRequestModel);
}

void WindowManager::registerTypes() {
    // Register any custom QML types here
    qmlRegisterType<QObject>("Client", 1, 0, "WindowManager");
}

void WindowManager::setupConnections() {
    // Connect to AuthController signals for navigation
    connect(controllerManager->authController, &AuthController::loginResult,
            this, [this](bool success, const QString &message) {
                if (success) {
                    showMainWindow();
                } else {
                    qWarning() << "Login failed:" << message;
                }
            });
    
    connect(controllerManager->authController, &AuthController::logoutRequest,
            this, &WindowManager::showAuthWindow);
}

void WindowManager::showMainWindow() {
    engine.load(QUrl(QStringLiteral("qrc:/src/views/MainWindow.qml")));
}

void WindowManager::showAuthWindow() {
    engine.load(QUrl(QStringLiteral("qrc:/src/views/AuthWindow.qml")));
}