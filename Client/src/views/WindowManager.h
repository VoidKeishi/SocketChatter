#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "../controllers/ControllerManager.h"

class WindowManager : public QObject {
    Q_OBJECT
public:
    static WindowManager* instance();
    void initializeQml();
    void setControllerManager(ControllerManager* manager);
    
    Q_INVOKABLE void showMainWindow();
    Q_INVOKABLE void showAuthWindow();

signals:
    void initialized();

private:
    explicit WindowManager(QObject *parent = nullptr);
    static WindowManager* m_instance;
    QQmlApplicationEngine engine;
    ControllerManager* controllerManager;

    void registerModels();
    void registerTypes();
    void setupConnections();
};

#endif // WINDOWMANAGER_H