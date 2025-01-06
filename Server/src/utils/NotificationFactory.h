#pragma once

#include <QJsonObject>
#include <QString>
#include <QDateTime>
#include <QJsonDocument>

namespace NotificationFactory
{
    inline QJsonObject createFriendRequestNotification(const QString &from, const QString &to)
    {
        return QJsonObject{
            {"type", "FRIEND_REQUEST_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"from", from},
                {"to", to}
            }}
        };
    }

    inline QJsonObject createFriendRequestCanceledNotification(const QString &from, const QString &to)
    {
        return QJsonObject{
            {"type", "FRIEND_REQUEST_CANCELED_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"from", from},
                {"to", to}
            }}
        };
    }

    inline QJsonObject createFriendRequestResponseNotification(const QString &from, const QString &to, bool accept)
    {
        return QJsonObject{
            {"type", "FRIEND_REQUEST_RESPONSE_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"from", from},
                {"to", to},
                {"accept", accept}
            }}
        };
    }

    inline QJsonObject createFriendDeletionNotification(const QString &from, const QString &to)
    {
        return QJsonObject{
            {"type", "FRIEND_DELETED_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"from", from},
                {"to", to}
            }}
        };
    }

    inline QJsonObject createMessageNotification(const QString &sender, const QString &receiver, const QString &content)
    {
        return QJsonObject{
            {"type", "MESSAGE_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"from", sender},
                {"to", receiver},
                {"content", content}
            }}
        };
    }

    inline QJsonObject createGroupInviteNotification(const QString& groupId, const QString& inviter, const QString& invitee)
    {
        return QJsonObject{
            {"type", "GROUP_INVITE_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"groupId", groupId},
                {"inviter", inviter},
                {"invitee", invitee}
            }}
        };
    }

    inline QJsonObject createGroupInviteResponseNotification(const QString& groupId, const QString& groupName, const QString& responder, bool accepted)
    {
        return QJsonObject{
            {"type", "GROUP_INVITE_RESPONSE_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"groupId", groupId},
                {"groupName", groupName},
                {"responder", responder},
                {"accepted", accepted}
            }}
        };
    }

    inline QJsonObject createGroupMemberLeftNotification(const QString& groupId, const QString& groupName, const QString& username)
    {
        return QJsonObject{
            {"type", "GROUP_MEMBER_LEFT_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"groupId", groupId},
                {"groupName", groupName},
                {"username", username}
            }}
        };
    }
    
}