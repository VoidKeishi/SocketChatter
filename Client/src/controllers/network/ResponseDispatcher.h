#ifndef RESPONSEDISPATCHER_H
#define RESPONSEDISPATCHER_H

#include <QObject>
#include <QByteArray>
#include <QJsonObject>

class ResponseDispatcher : public QObject {
    Q_OBJECT
public:
    explicit ResponseDispatcher(QObject *parent = nullptr);

public slots:
    void onRawDataReceived(const QByteArray &data);

signals:
    // Auth Responses
    void loginResponseReceived(const QJsonObject &response);
    void registerResponseReceived(const QJsonObject &response);

    // Contacts Responses
    void friendRequestSentResponse(const QJsonObject &response);
    void friendRequestCanceledResponse(const QJsonObject &response);
    void friendRequestResponse(const QJsonObject &response);
    void friendDeletedResponse(const QJsonObject &response);
    void friendsListFetchedResponse(const QJsonObject &response);

private:
    void dispatchResponse(const QByteArray &data);
};

#endif // RESPONSEDISPATCHER_H