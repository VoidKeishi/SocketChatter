#include "ResponseDispatcher.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

ResponseDispatcher::ResponseDispatcher(QObject *parent) : QObject(parent) {}

void ResponseDispatcher::registerController(IController* controller) {
    if (!controller) {
        qWarning() << "Attempting to register null controller";
        return;
    }
    controllers.append(controller);
}

void ResponseDispatcher::onRawDataReceived(const QByteArray& data) {
    dispatchResponse(data);
}

void ResponseDispatcher::dispatchResponse(const QByteArray& data) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << error.errorString();
        return;
    }

    QJsonObject response = doc.object();
    QString type = response["type"].toString();
    QJsonObject payload = response["payload"].toObject();

    qDebug() << "Dispatching response type:" << type;

    for (IController* controller : controllers) {
        if (controller->canHandle(type)) {
            controller->handle(type, payload);
            return;
        }
    }

    qWarning() << "No handler found for response type:" << type;
}