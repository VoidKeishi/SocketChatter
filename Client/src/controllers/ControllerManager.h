#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <QObject>

#include "auth/AuthController.h"
#include "contacts/ContactsController.h"
#include "messages/MessagesController.h"
#include "network/ResponseDispatcher.h"
#include "../viewmodels/AuthViewModel.h"
#include "../viewmodels/ContactViewModel.h"
#include "../viewmodels/ConversationViewModel.h"

class NetworkController;

class ControllerManager : public QObject {
    Q_OBJECT
public:
    explicit ControllerManager(QObject *parent = nullptr);
    static ControllerManager* instance();

    // Public controllers for QML
    AuthController* authController;
    ContactsController* contactsController;
    MessagesController* messagesController;

    // ViewModel getters
    AuthViewModel* authViewModel() const { return m_authViewModel; }
    ContactViewModel* contactViewModel() const { return m_contactViewModel; }
    ConversationViewModel* conversationViewModel() const { return m_conversationViewModel; }

private:
    static ControllerManager* m_instance;
    ResponseDispatcher* responseDispatcher;
    NetworkController* networkController;

    // Private ViewModels
    AuthViewModel* m_authViewModel;
    ContactViewModel* m_contactViewModel;
    ConversationViewModel* m_conversationViewModel;
};

#endif // CONTROLLERMANAGER_H