#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>

class LoginController : public QObject {
    Q_OBJECT
public:
    explicit LoginController(QObject *parent = nullptr);

signals:
    void loginResult(bool success, QString message);

public slots:
    void handleLoginRequest(const QString &username, const QString &password);

private slots:
    void onLoginResponse(bool success, const QString &message);
};

#endif // LOGINCONTROLLER_H