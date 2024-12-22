#include "RequestDispatcher.h"

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
    qDebug() << "Dispatching request type:" << type;
    
    for (BaseHandler* handler : handlers) {
        if (handler->canHandle(type)) {
            qDebug() << "Handler found for type:" << type;
            handler->handle(type, request);
            return;
        }
    }
    
    qWarning() << "No handler found for type:" << type;
    emit error(QString("No handler found for request type: %1").arg(type));
}