#include "ControllerManager.h"
#include <QDebug>

ControllerManager::ControllerManager(QObject *parent) : QObject(parent) {
    loginController = new LoginController(this);
    registerController = new RegisterController(this);
    manageContactsController = new ManageContactsController(this);
}

void ControllerManager::initControllers() {

    QObject::connect(loginController, &LoginController::loginResult, [=](bool success, QString message) {
        if (!success) {
            qDebug() << "Login failed:" << message;
            return;
        }
        qDebug() << "Login successful!";
    });

    QObject::connect(registerController, &RegisterController::registerResult, [=](bool success, QString message) {
        if (!success) {
            qDebug() << "Registration failed:" << message;
            return;
        }
        qDebug() << "Registration successful!";
    });

    QObject::connect(manageContactsController, &ManageContactsController::friendRequestSent, 
                     [=](bool success, QString message) {
        if (!success) {
            qDebug() << "Friend request failed:" << message;
            // Handle failure (e.g., show error message in UI)
            return;
        }
        qDebug() << "Friend request sent successfully!";
        // Handle success (e.g., update UI)
    });

    QObject::connect(manageContactsController, &ManageContactsController::friendRequestCanceled, 
                     [=](bool success, QString message) {
        if (!success) {
            qDebug() << "Cancel friend request failed:" << message;
            // Handle failure
            return;
        }
        qDebug() << "Friend request canceled successfully!";
        // Handle success
    });

    QObject::connect(manageContactsController, &ManageContactsController::friendRequestAccepted, 
                     [=](bool success, QString message) {
        if (!success) {
            qDebug() << "Accept friend request failed:" << message;
            // Handle failure
            return;
        }
        qDebug() << "Friend request accepted successfully!";
        // Handle success
    });

    QObject::connect(manageContactsController, &ManageContactsController::friendRequestRejected, 
                     [=](bool success, QString message) {
        if (!success) {
            qDebug() << "Reject friend request failed:" << message;
            // Handle failure
            return;
        }
        qDebug() << "Friend request rejected successfully!";
        // Handle success
    });

    QObject::connect(manageContactsController, &ManageContactsController::friendDeleted, 
                     [=](bool success, QString message) {
        if (!success) {
            qDebug() << "Delete friend failed:" << message;
            // Handle failure
            return;
        }
        qDebug() << "Friend deleted successfully!";
        // Handle success
    });

    QObject::connect(manageContactsController, &ManageContactsController::friendsListFetched, 
                     [=](const QStringList &friends) {
        qDebug() << "Fetched friends list:" << friends;
        // Update UI with the friends list
    });
}