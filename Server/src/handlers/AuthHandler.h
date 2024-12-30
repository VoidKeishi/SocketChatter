#pragma once
#include "BaseHandler.h"
#include "../repositories/UserRepository.h"

class AuthHandler : public BaseHandler {
    Q_OBJECT
public:
    explicit AuthHandler(DatabaseManager* db);

private:
    UserRepository userRepo;
    void handleLogin(const QJsonObject& payload);
    void handleRegister(const QJsonObject& payload);
};