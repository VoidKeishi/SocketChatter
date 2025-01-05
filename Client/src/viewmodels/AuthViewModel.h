#pragma once

#include <QObject>

enum class AuthAction {
    Login,
    Register,
};

class AuthViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRegistered READ isRegistered NOTIFY registeredChanged)
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(int currentTab READ currentTab WRITE setCurrentTab NOTIFY currentTabChanged)

public:
    explicit AuthViewModel(QObject* parent = nullptr);
    bool isRegistered() const { return m_registered; }
    bool isLoggedIn() const { return m_loggedIn; }
    QString message() const { return m_message; }
    int currentTab() const { return m_currentTab; }

public slots:
    // Setters
    void setRegistered(bool registered);
    void setLoggedIn(bool loggedIn);
    void setMessage(const QString& message);
    void setCurrentTab(int tab);
    void reset();
    // Methods called by View
    void sendRequestLogin(const QString& username, const QString& password);
    void sendRequestRegister(const QString& username, const QString& password);

signals:
    void registeredChanged();
    void loggedInChanged();
    void messageChanged();
    void currentTabChanged();
    void authActionRequested(AuthAction action, const QString& username, const QString& password);

private:
    bool m_registered;
    bool m_loggedIn;
    QString m_message;
    int m_currentTab;
};