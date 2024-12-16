#ifndef REGISTERCONTROLLER_H
#define REGISTERCONTROLLER_H

#include <QObject>

class RegisterController : public QObject {
    Q_OBJECT
public:
    explicit RegisterController(QObject *parent = nullptr);

signals:
    void registerResult(bool success, QString message);

public slots:
    void handleRegisterRequest(const QString &username, const QString &password);

private slots:
    void onRegisterResponse(bool success, const QString &message);
};

#endif // REGISTERCONTROLLER_H