#include "ControllerManager.h"

ControllerManager* ControllerManager::m_instance = nullptr;

ControllerManager::ControllerManager(QObject *parent) : QObject(parent) {
        m_authViewModel = new AuthViewModel(this);
        authController = new AuthController(m_authViewModel, this);
        
        m_contactViewModel = new ContactViewModel(this);
        contactsController = new ContactsController(m_contactViewModel, this);

        m_conversationViewModel = new ConversationViewModel(this);
        messagesController = new MessagesController(m_conversationViewModel, this);

        m_groupViewModel = new GroupViewModel(this);
        groupsController = new GroupsController(m_groupViewModel, this);

        // Instantiate ResponseDispatcher
        responseDispatcher = new ResponseDispatcher(this);
        responseDispatcher->registerController(authController);
        responseDispatcher->registerController(contactsController);
        responseDispatcher->registerController(messagesController);
        responseDispatcher->registerController(groupsController);
}

ControllerManager* ControllerManager::instance() {
        if (!m_instance) {
                m_instance = new ControllerManager(nullptr);
        }
        return m_instance;
}