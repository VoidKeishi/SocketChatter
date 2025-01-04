#include "ControllerManager.h"
#include "network/NetworkController.h"
#include <QDebug>

ControllerManager* ControllerManager::m_instance = nullptr;

ControllerManager::ControllerManager(QObject *parent) : QObject(parent) {
        m_authViewModel = new AuthViewModel(this);
        authController = new AuthController(m_authViewModel, this);
        
        m_contactViewModel = new ContactViewModel(this);
        contactsController = new ContactsController(m_contactViewModel, this);

        m_conversationViewModel = new ConversationViewModel(this);
        messagesController = new MessagesController(m_conversationViewModel, this);

        // Instantiate ResponseDispatcher
        responseDispatcher = new ResponseDispatcher(this);
        responseDispatcher->registerController(authController);
        responseDispatcher->registerController(contactsController);
        responseDispatcher->registerController(messagesController);

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