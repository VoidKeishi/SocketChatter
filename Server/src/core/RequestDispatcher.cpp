#include "RequestDispatcher.h"
#include "../utils/Logger.h"

RequestDispatcher::RequestDispatcher(QObject* parent) : QObject(parent) {}

void RequestDispatcher::registerHandler(BaseHandler* handler) {
    handlers.append(handler);
    
    // Connect handler signals to dispatcher signals
    connect(handler, &BaseHandler::responseReady, 
            this, &RequestDispatcher::responseReady);
    connect(handler, &BaseHandler::error,
            this, &RequestDispatcher::error);
}

void RequestDispatcher::dispatch(const QJsonObject& request) {
    QString type = request["type"].toString();
    Logger::debug("Dispatching request type: " + type);
    
    for (BaseHandler* handler : handlers) {
        if (handler->canHandle(type)) {
            Logger::debug("Handler found for type: " + type);
            handler->handle(type, request);
            return;
        }
    }
    
    Logger::error("No handler found for request type: " + type);
    emit error(QString("No handler found for request type: %1").arg(type));
}