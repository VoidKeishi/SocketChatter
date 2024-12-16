#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <QObject>
#include "auth/LoginController.h"
#include "auth/RegisterController.h"
#include "contacts/ManageContactsController.h"

class ControllerManager : public QObject {
    Q_OBJECT
public:
    explicit ControllerManager(QObject *parent = nullptr);
    void initControllers();

    LoginController *loginController;
    RegisterController *registerController;
    ManageContactsController *manageContactsController;
};

#endif // CONTROLLERMANAGER_H