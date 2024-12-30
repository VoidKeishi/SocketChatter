#pragma once
#include "../IController.h"
#include "../../viewmodels/ContactViewModel.h"
#include <QObject>
#include <QStringList>
#include <QMap>

class ContactsController : public QObject, public IController {
    Q_OBJECT

public:
    explicit ContactsController(ContactViewModel* viewModel, QObject* parent = nullptr);
    bool canHandle(const QString& type) const override;
    void handle(const QString& type, const QJsonObject& payload) override;
    void requestAddFriend(const QString &username);
    void requestCancelFriend(const QString &username);
    void requestRespondToFriend(const QString &username, bool accept);
    void requestDeleteFriend(const QString &username);
    void requestFetchFriends();
    void requestFetchSentRequests();
    void requestFetchReceivedRequests();

signals:
    // Signals to send requests to server
    void sendRequest(const QByteArray &data);

public slots:
    void handleContactAction(ContactAction action, const QString& username, bool param);

private:
    ContactViewModel* m_viewModel;
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;

    // Slots to handle responses from server
    void handleFriendRequestSent(const QJsonObject &response);
    void handleFriendRequestCanceled(const QJsonObject &response);
    void handleFriendRequestResponse(const QJsonObject &response);
    void handleFriendsListFetched(const QJsonObject &response);
    void handleFriendDeleted(const QJsonObject &response);
    void handleSentListFetched(const QJsonObject& response);
    void handleReceivedListFetched(const QJsonObject& response);
};