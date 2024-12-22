#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QString>

class AuthController : public QObject {
    Q_OBJECT
public:
    explicit AuthController(QObject *parent = nullptr);
    Q_INVOKABLE void handleLoginRequest(const QString &username, const QString &password);
    Q_INVOKABLE void handleRegisterRequest(const QString &username, const QString &password);
    Q_INVOKABLE void logout();

public slots:
    void onLoginResponse(const QJsonObject &response);
    void onRegisterResponse(const QJsonObject &response);

signals:
    void loginRequest(const QByteArray &data);
    void registerRequest(const QByteArray &data);
    void loginResult(bool success, const QString &message);
    void registerResult(bool success, const QString &message);
    void logoutRequest();
};

#endif // AUTHCONTROLLER_H