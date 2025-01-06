#pragma once

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <functional>

#include "../../viewmodels/ConversationViewModel.h"

class MessagesResponseHandler : public QObject {
    Q_OBJECT

public:
    explicit MessagesResponseHandler(ConversationViewModel* viewModel, QObject* parent = nullptr);

public slots:
    void handleFetchMessagesResponse(const QJsonObject& response);
    void handleSendMessageResponse(const QJsonObject& response);

private:
    ConversationViewModel* m_viewModel;
};