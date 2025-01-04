#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

#include "ResponseDispatcher.h"
#include "NetworkController.h"
#include "../utils/Logger.h"

ResponseDispatcher::ResponseDispatcher(QObject *parent) : QObject(parent) {
    // Connect to NetworkController's rawDataReceived signal
    connect(NetworkController::instance(), &NetworkController::rawDataReceived,
            this, &ResponseDispatcher::onRawDataReceived);
}

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
    Logger::json("Received response", response);
    
    QString type = response["type"].toString();
    QJsonObject payload = response["payload"].toObject();
    Logger::debug("Dispatching response type: " + type);

    for (IController* controller : controllers) {
        if (controller->canHandle(type)) {
            controller->handle(type, payload);
            return;
        }
    }

    Logger::error("No controller found for response type: " + type);
}