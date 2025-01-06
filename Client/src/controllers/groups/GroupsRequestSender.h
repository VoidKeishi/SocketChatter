#pragma once

#include <QObject>
#include <QJsonObject>

class GroupsRequestSender : public QObject {
    Q_OBJECT

public:
    explicit GroupsRequestSender(QObject* parent = nullptr);

public slots:
    void requestCreateGroup(const QString& sender, const QString& groupName, const QString& groupDetails = "");
    void requestFetchGroups(const QString& sender);
    void requestDeleteGroup(const QString& sender, const QString& groupName);
    void requestInviteToGroup(const QString& sender, const QString& groupName, const QString& invitee);
    void requestRespondToInvitation(const QString& sender, const QString& groupName, bool accept);

signals:
    void sendRequest(const QByteArray& data);
};