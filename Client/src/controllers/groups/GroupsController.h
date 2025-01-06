#pragma once

#include <QObject>
#include <QMap>
#include <functional>
#include <QJsonArray>

#include "GroupsRequestSender.h"
#include "GroupsResponseHandler.h"
#include "GroupsNotificationHandler.h"
#include "../IController.h"
#include "../../viewmodels/ConversationViewModel.h"
#include "../session/UserManager.h"
#include "../utils/RequestFactory.h"
#include "../utils/Logger.h"
#include "../network/NetworkController.h"

class GroupsController : public QObject, public IController {
    Q_OBJECT
public:
    explicit GroupsController(ConversationViewModel* viewModel, QObject* parent = nullptr);
    // IController interface
    bool canHandle(const QString& type) const override;
    void handle(const QString& type, const QJsonObject& payload) override;

public slots:
    void sendMessage(const QString& sender, const QString& receiver, const QString& content);
    void fetchGroups(const QString& sender, const QString& receiver);

    void handleMessageAction(MessageAction action, const QString& sender, const QString& receiver, const QString& content);

signals:
    void sendRequest(const QByteArray& data);

private:
    ConversationViewModel* m_viewModel;
    GroupsRequestSender* m_requestSender;
    GroupsResponseHandler* m_responseHandler;
    GroupsNotificationHandler* m_notificationHandler;
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;
};