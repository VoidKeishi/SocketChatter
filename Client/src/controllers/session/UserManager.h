#pragma once
#include <QString>
#include <QObject>

class UserManager : public QObject {
    Q_OBJECT
public:
    static UserManager* instance();
    void setCurrentUser(const QString& username);
    QString currentUser() const { return m_currentUser; }

private:
    explicit UserManager(QObject* parent = nullptr);
    static UserManager* m_instance;
    QString m_currentUser;
};