#pragma once

#include <QObject>
#include <QStringList>

enum class GroupAction {
    CreateGroup,
    FetchGroups,
    DeleteGroup,
    InviteGroupMember,
};

class GroupViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList groups READ groups NOTIFY groupsChanged)
    Q_PROPERTY(QStringList receivedInvitations READ receivedInvitations NOTIFY receivedInvitationsChanged)

public:
    explicit GroupViewModel(QObject* parent = nullptr);
    // Add getters for properties (required for QML)
    QStringList groups() const { return m_groups; }
    QStringList receivedInvitations() const { return m_receivedInvitations; }

    // Updaters
    Q_INVOKABLE void addGroupInvitation(const QString& group);
    Q_INVOKABLE void removeGroupInvitation(const QString& group);
    Q_INVOKABLE void addGroup(const QString& group);
    Q_INVOKABLE void removeGroup(const QString& group);


public slots:
    void sendGroupCreateRequest(const QString& groupName);
    void sendGroupInvitation(const QString& groupName, const QString& friendName);
    void respondGroupInvitation(const QString& groupName, bool accept);
    void fetchGroups();
    void fetchGroupInvitations();

signals:
    void groupActionRequested(GroupAction action, const QString& username, const QString& groupName, const QString& member = "");
    void groupsChanged();
    void receivedInvitationsChanged();
    void groupActionSuccess(const QString& message);
    void groupActionError(const QString& error);

private:
    QStringList m_groups;
    QStringList m_receivedInvitations;
};