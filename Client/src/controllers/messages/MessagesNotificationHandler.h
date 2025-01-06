#pragma once

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <functional>

#include "../../viewmodels/ConversationViewModel.h"

class MessagesNotificationHandler : public QObject {
    Q_OBJECT

public:
    explicit MessagesNotificationHandler(ConversationViewModel* viewModel, QObject* parent = nullptr);

public slots:
    void handleMessageNotification(const QJsonObject& notification);

private:
    ConversationViewModel* m_viewModel;
};