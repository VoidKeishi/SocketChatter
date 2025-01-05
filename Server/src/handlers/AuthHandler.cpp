#include "AuthHandler.h"
#include "../utils/Logger.h"
#include "../utils/ResponseFactory.h"

AuthHandler::AuthHandler(DatabaseManager* db) 
    : BaseHandler(nullptr), userRepo(db) {
    handlers = {
        {"LOGIN_REQUEST", [this](const QJsonObject& p) { handleLogin(p); }},
        {"REGISTER_REQUEST", [this](const QJsonObject& p) { handleRegister(p); }}
    };
}

void AuthHandler::handleLogin(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    QString passwordHash = payload["password"].toString();

    Logger::debug(QString("Processing login for user: %1").arg(username));

    if (userRepo.validateUser(username, passwordHash)) {
        emit responseReady(ResponseFactory::createLoginResponse(true, "Login successful", username));
        emit loginSuccess(username);
    } else {
        emit responseReady(ResponseFactory::createLoginResponse(false, "Invalid credentials"));
    }
}

void AuthHandler::handleRegister(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    QString passwordHash = payload["password"].toString();

    Logger::debug(QString("Processing registration for user: %1").arg(username));
    
    if (userRepo.userExists(username)) {
        emit responseReady(ResponseFactory::createRegisterResponse(false, "Username already exists"));
        return;
    }

    if (userRepo.createUser(username, passwordHash)) {
        emit responseReady(ResponseFactory::createRegisterResponse(true, "Registration successfull"));
    } else {
        emit responseReady(ResponseFactory::createRegisterResponse(false, "Failed to create user"));
    }
}