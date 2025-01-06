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
}