#pragma once

#include <QObject>
#include <QMap>
#include <functional>
#include <QJsonArray>

#include "MessagesRequestSender.h"
#include "MessagesResponseHandler.h"
#include "MessagesNotificationHandler.h"
#include "../IController.h"
#include "../../viewmodels/ConversationViewModel.h"
#include "../session/UserManager.h"
#include "../utils/RequestFactory.h"
#include "../utils/Logger.h"
#include "../network/NetworkController.h"

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

    void handleMessageAction(MessageAction action, const QString& sender, const QString& receiver, const QString& content);

signals:
    void sendRequest(const QByteArray& data);

private:
    ConversationViewModel* m_viewModel;
    MessagesRequestSender* m_requestSender;
    MessagesResponseHandler* m_responseHandler;
    MessagesNotificationHandler* m_notificationHandler;
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;
};