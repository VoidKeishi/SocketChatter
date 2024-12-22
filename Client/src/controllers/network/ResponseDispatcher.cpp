#include "ResponseDispatcher.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>

ResponseDispatcher::ResponseDispatcher(QObject *parent) : QObject(parent) {}

void ResponseDispatcher::onRawDataReceived(const QByteArray &data) {
    dispatchResponse(data);
}

void ResponseDispatcher::dispatchResponse(const QByteArray &data) {
    qDebug() << "Raw response data:" << QString(data);
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return;
    }

    QJsonObject jsonObj = doc.object();
    QString type = jsonObj.value("type").toString();

    if (type == "LOGIN_RESPONSE") {
        emit loginResponseReceived(jsonObj);
    }
    else if (type == "REGISTER_RESPONSE") {
        emit registerResponseReceived(jsonObj);
    }
    else if (type == "FRIEND_REQUEST_SENT_RESPONSE") {
        emit friendRequestSentResponse(jsonObj);
    }
    else if (type == "FRIEND_REQUEST_CANCELED_RESPONSE") {
        emit friendRequestCanceledResponse(jsonObj);
    }
    else if (type == "FRIEND_REQUEST_RESPONSE") {
        emit friendRequestResponse(jsonObj);
    }
    else if (type == "FRIEND_DELETED_RESPONSE") {
        emit friendDeletedResponse(jsonObj);
    }
    else if (type == "FETCH_FRIENDS_LIST_RESPONSE") {
        emit friendsListFetchedResponse(jsonObj);
    }
    else {
        qWarning() << "Unknown response type:" << type;
    }
}