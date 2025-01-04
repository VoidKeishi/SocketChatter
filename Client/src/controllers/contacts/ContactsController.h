#pragma once

#include <QObject>
#include <QStringList>
#include <QMap>

#include "ContactsRequestSender.h"
#include "ContactsResponseHandler.h"
#include "../IController.h"
#include "../../viewmodels/ContactViewModel.h"

class ContactsController : public QObject, public IController {
    Q_OBJECT

public:
    explicit ContactsController(ContactViewModel* viewModel, QObject* parent = nullptr);
    // IController interface
    bool canHandle(const QString& type) const override;
    void handle(const QString& type, const QJsonObject& payload) override;

public slots:
    void handleContactAction(ContactAction action, const QString& username, bool param);

private:
    ContactViewModel* m_viewModel;
    ContactsRequestSender* m_requestSender;
    ContactsResponseHandler* m_responseHandler;
    QMap<QString, std::function<void(const QJsonObject&)>> handlers;
};