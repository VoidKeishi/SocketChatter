#include "ControllerManager.h"
#include "network/NetworkController.h"
#include <QDebug>

ControllerManager* ControllerManager::m_instance = nullptr;

ControllerManager::ControllerManager(QObject *parent) : QObject(parent) {
        m_authViewModel = new AuthViewModel(this);
        m_contactViewModel = new ContactViewModel(this);

        authController = new AuthController(m_authViewModel, this);
        contactsController = new ContactsController(m_contactViewModel, this);

        // Instantiate ResponseDispatcher
        responseDispatcher = new ResponseDispatcher(this);
        responseDispatcher->registerController(authController);
        responseDispatcher->registerController(contactsController);

        // Get the singleton instance of NetworkController
        networkController = NetworkController::instance();
        networkController->connectToServer("localhost", 3000);
}

ControllerManager* ControllerManager::instance() {
        if (!m_instance) {
                m_instance = new ControllerManager(nullptr);
        }
        return m_instance;
}

void ControllerManager::initControllers() {
        // Connect NetworkController to ResponseDispatcher
        connect(networkController, &NetworkController::rawDataReceived,
                        responseDispatcher, &ResponseDispatcher::onRawDataReceived);

        // AuthController Signals
        connect(authController, &AuthController::sendRequest,
                        networkController, &NetworkController::sendData);

        // ContactsController Signals
        connect(contactsController, &ContactsController::sendRequest,
                        networkController, &NetworkController::sendData);
}
