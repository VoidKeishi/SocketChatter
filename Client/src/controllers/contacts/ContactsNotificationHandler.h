#pragma once

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <functional>

#include "../../viewmodels/ContactViewModel.h"

class ContactsNotificationHandler : public QObject {
    Q_OBJECT

public:
    explicit ContactsNotificationHandler(ContactViewModel* viewModel, QObject* parent = nullptr);

public slots:
    void handleFriendRequestNotification(const QJsonObject& notification);
    void handleFriendRequestCanceledNotification(const QJsonObject& notification);
    void handleFriendRequestResponseNotification(const QJsonObject& notification);
    void handleFriendDeletedNotification(const QJsonObject& notification);

private:
    ContactViewModel* m_viewModel;
};