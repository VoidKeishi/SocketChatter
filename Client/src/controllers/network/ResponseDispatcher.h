#ifndef RESPONSEDISPATCHER_H
#define RESPONSEDISPATCHER_H

#include <QObject>
#include <QByteArray>
#include <QJsonObject>
#include <QMap>
#include "../IController.h"


class ResponseDispatcher : public QObject {
    Q_OBJECT
public:
    explicit ResponseDispatcher(QObject *parent = nullptr);
    void registerController(IController* controller);

public slots:
    void onRawDataReceived(const QByteArray &data);

private:
    void dispatchResponse(const QByteArray &data);
    QVector<IController*> controllers;
};

#endif // RESPONSEDISPATCHER_H