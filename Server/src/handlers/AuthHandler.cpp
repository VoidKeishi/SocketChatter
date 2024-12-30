#include "AuthHandler.h"
#include <QDebug>

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

    qDebug() << "Processing login for user:" << username;

    if (userRepo.validateUser(username, passwordHash)) {
        emit responseReady({
            {"type", "LOGIN_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", true},
                {"message", "Login successful"},
                {"username", username}
            }}
        });
    } else {
        emit responseReady({
            {"type", "LOGIN_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Invalid credentials"}
            }}
        });
    }
}

void AuthHandler::handleRegister(const QJsonObject& request) {
    QJsonObject payload = request["payload"].toObject();
    QString username = payload["username"].toString();
    QString passwordHash = payload["password"].toString();

    qDebug() << "Processing registration for user:" << username;
    
    if (userRepo.userExists(username)) {
        emit responseReady({
            {"type", "REGISTER_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Username already exists"}
            }}
        });
        return;
    }

    if (userRepo.createUser(username, passwordHash)) {
        emit responseReady({
            {"type", "REGISTER_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", true},
                {"message", "Registration successful"}
            }}
        });
    } else {
        emit responseReady({
            {"type", "REGISTER_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", false},
                {"message", "Failed to create user"}
            }}
        });
    }
}