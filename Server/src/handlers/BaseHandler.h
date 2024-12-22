#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <QObject>
#include <QJsonObject>
#include <functional>
#include <QMap>

class BaseHandler : public QObject {
    Q_OBJECT
public:
    explicit BaseHandler(QObject* parent = nullptr) : QObject(parent) {}
    virtual bool canHandle(const QString& type) const {
        return handlers.contains(type);
    }
    
    virtual void handle(const QString& type, const QJsonObject& request) {
        if (handlers.contains(type)) {
            handlers[type](request);
        }
    }

signals:
    void responseReady(const QJsonObject& response);
    void error(const QString& message);

protected:
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;
};

#endif // BASEHANDLER_H