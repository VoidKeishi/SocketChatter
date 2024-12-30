#pragma once
#include <QObject>

enum class AuthStatus {
    Idle,
    Loading,
    Success,
    Error
};

class AuthViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY loadingChanged)
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(bool isRegistered READ isRegistered NOTIFY registeredChanged)
    Q_PROPERTY(int currentTab READ currentTab WRITE setCurrentTab NOTIFY currentTabChanged)

public:
    explicit AuthViewModel(QObject* parent = nullptr);

    bool isLoading() const { return m_status == AuthStatus::Loading; }
    bool isLoggedIn() const { return m_loggedIn; }
    QString message() const { return m_message; }
    bool isRegistered() const { return m_registered; }
    int currentTab() const { return m_currentTab; }

public slots:
    void setLoading(bool loading);
    void setLoggedIn(bool loggedIn);
    void setMessage(const QString& message);
    void setRegistered(bool registered);
    void setCurrentTab(int tab);
    void reset();

signals:
    void loadingChanged();
    void loggedInChanged();
    void messageChanged();
    void registeredChanged();
    void currentTabChanged();

private:
    AuthStatus m_status;
    bool m_loggedIn;
    QString m_message;
    bool m_registered;
    int m_currentTab;
};