#include "ControllerManager.h"
#include "network/NetworkController.h"
#include <QDebug>

ControllerManager::ControllerManager(QObject *parent) : QObject(parent) {
        // Instantiate Controllers
        authController = new AuthController(this);
        contactsController = new ContactsController(this);

        // Instantiate Models
        friendListModel = new FriendListModel(this);
        sentRequestModel = new SentRequestModel(this);
        receivedRequestModel = new ReceivedRequestModel(this);

        // Instantiate ResponseDispatcher
        responseDispatcher = new ResponseDispatcher(this);

        // Get the singleton instance of NetworkController
        networkController = NetworkController::instance();
}

ControllerManager* ControllerManager::instance() {
        if (!m_instance) {
                m_instance = new ControllerManager(nullptr);
        }
        return m_instance;
}

void ControllerManager::initControllers() {
        // =========================================
        // 1. Connect NetworkController to ResponseDispatcher
        // =========================================
        connect(networkController, &NetworkController::rawDataReceived,
                        responseDispatcher, &ResponseDispatcher::onRawDataReceived);

        // =========================================
        // 2. Connect ResponseDispatcher to AuthController
        // =========================================
        connect(responseDispatcher, &ResponseDispatcher::loginResponseReceived,
                        authController, &AuthController::onLoginResponse);
        connect(responseDispatcher, &ResponseDispatcher::registerResponseReceived,
                        authController, &AuthController::onRegisterResponse);

        // =========================================
        // 3. Connect ResponseDispatcher to ContactsController
        // =========================================
        connect(responseDispatcher, &ResponseDispatcher::friendRequestSentResponse,
                        contactsController, &ContactsController::handleFriendRequestSent);
        connect(responseDispatcher, &ResponseDispatcher::friendRequestCanceledResponse,
                        contactsController, &ContactsController::handleFriendRequestCanceled);
        connect(responseDispatcher, &ResponseDispatcher::friendRequestResponse,
                        contactsController, &ContactsController::handleFriendRequestResponse);
        connect(responseDispatcher, &ResponseDispatcher::friendsListFetchedResponse,
                        contactsController, &ContactsController::handleFriendsListFetched);

        // =========================================
        // 4. Connect Controllers' Send Signals to NetworkController's sendData
        // =========================================
        // AuthController Signals
        connect(authController, &AuthController::loginRequest,
                        networkController, &NetworkController::sendData);
        connect(authController, &AuthController::registerRequest,
                        networkController, &NetworkController::sendData);

        // ContactsController Signals
        connect(contactsController, &ContactsController::sendAddFriend,
                        networkController, &NetworkController::sendData);
        connect(contactsController, &ContactsController::sendCancelFriend,
                        networkController, &NetworkController::sendData);
        connect(contactsController, &ContactsController::sendRespondToFriend,
                        networkController, &NetworkController::sendData);
        connect(contactsController, &ContactsController::sendDeleteFriend,
                        networkController, &NetworkController::sendData);
        connect(contactsController, &ContactsController::sendFetchFriends,
                        networkController, &NetworkController::sendData);

        // =========================================
        // 5. Connect ContactsController signals to models
        // =========================================
        connect(contactsController, &ContactsController::friendsListResult,
                        friendListModel, &FriendListModel::updateFriendsList);
                        
        connect(contactsController, &ContactsController::addFriendResult,
                        sentRequestModel, &SentRequestModel::onFriendRequestSent);
                        
        connect(contactsController, &ContactsController::friendResponseResult,
                        receivedRequestModel, &ReceivedRequestModel::onFriendRequestResponse);
}
