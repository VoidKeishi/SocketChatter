#pragma once
#include "../handlers/BaseHandler.h"
#include <QVector>
#include <QObject>

class RequestDispatcher : public QObject {
    Q_OBJECT
public:
    explicit RequestDispatcher(QObject* parent = nullptr);
    void registerHandler(BaseHandler* handler);
    void dispatch(const QJsonObject& request);

signals:
    void responseReady(const QJsonObject& response);
    void error(const QString& message);

private:
    QVector<BaseHandler*> handlers;
};