#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <QObject>
#include "auth/AuthController.h"
#include "contacts/ContactsController.h"
#include "network/ResponseDispatcher.h"
#include "../models/contacts/FriendListModel.h"
#include "../models/contacts/SentRequestModel.h"
#include "../models/contacts/ReceivedRequestModel.h"

class NetworkController;

class ControllerManager : public QObject {
    Q_OBJECT
public:
    explicit ControllerManager(QObject *parent = nullptr);
    static ControllerManager* instance();
    void initControllers();

    // Controllers
    AuthController *authController;
    ContactsController *contactsController;

    // Models
    FriendListModel *friendListModel;
    SentRequestModel *sentRequestModel;
    ReceivedRequestModel *receivedRequestModel;

private:
    static ControllerManager* m_instance;
    ResponseDispatcher *responseDispatcher;
    NetworkController* networkController;
};

#endif // CONTROLLERMANAGER_H