#pragma once

#include <QObject>
#include <QString>

class AuthRequestSender : public QObject 
{
    Q_OBJECT

public:
    explicit AuthRequestSender(QObject* parent = nullptr);

public slots:
    void requestLogin(const QString& username, const QString& password);
    void requestRegister(const QString& username, const QString& password);

signals:
    void sendRequest(const QByteArray& data);
};