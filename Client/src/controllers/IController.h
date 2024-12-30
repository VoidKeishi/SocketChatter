#pragma once
#include <QJsonObject>

class IController {
public:
    virtual ~IController() = default;
    virtual bool canHandle(const QString& type) const = 0;
    virtual void handle(const QString& type, const QJsonObject& payload) = 0;
};