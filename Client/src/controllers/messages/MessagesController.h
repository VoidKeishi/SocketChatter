#pragma once

#include "../IController.h"
#include "../../viewmodels/ConversationViewModel.h"
#include "../session/UserManager.h"
#include "../utils/RequestFactory.h"
#include "../utils/Logger.h"
#include "../network/NetworkController.h"
#include <QObject>
#include <QMap>
#include <functional>
#include <QJsonArray>

class MessagesController : public QObject, public IController {
    Q_OBJECT
public:
    explicit MessagesController(ConversationViewModel* viewModel, QObject* parent = nullptr);

    // IController interface
    bool canHandle(const QString& type) const override;
    void handle(const QString& type, const QJsonObject& payload) override;

public slots:
    void sendMessage(const QString& sender, const QString& receiver, const QString& content);
    void fetchMessages(const QString& sender, const QString& receiver);

signals:
    void sendRequest(const QByteArray& data);

private:
    ConversationViewModel* m_viewModel;
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;

    void handleSendMessageResponse(const QJsonObject& response);
    void handleFetchMessagesResponse(const QJsonObject& response);
};