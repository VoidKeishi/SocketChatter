#pragma once

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <functional>

#include "../../viewmodels/ContactViewModel.h"

class ContactsResponseHandler : public QObject {
    Q_OBJECT

public:
    explicit ContactsResponseHandler(ContactViewModel* viewModel, QObject* parent = nullptr);

public slots:
    void handleFriendRequestSent(const QJsonObject& response);
    void handleFriendRequestCanceled(const QJsonObject& response);
    void handleFriendRequestResponse(const QJsonObject& response);
    void handleFriendsListFetched(const QJsonObject& response);
    void handleFriendDeleted(const QJsonObject& response);
    void handleSentListFetched(const QJsonObject& response);
    void handleReceivedListFetched(const QJsonObject& response);

private:
    ContactViewModel* m_viewModel;
};