#ifndef RESPONSE_FACTORY_H
#define RESPONSE_FACTORY_H

#include <QJsonObject> 
#include <QString> 
#include <QDateTime> 
#include <QJsonDocument> 

namespace ResponseFactory
{
    // AuthHandler 
    inline QJsonObject createLoginResponse(bool success, const QString &message, const QString &username = QString())
    {
        return QJsonObject {
            {"type", "LOGIN_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"username", username}
            }}
        };
    }

    inline QJsonObject createRegisterResponse(bool success, const QString &message)
    {
        return QJsonObject {
            {"type", "REGISTER_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message}
            }}
        };
    }

    inline QJsonObject createFriendRequestResponse(bool success, const QString &message, const QString &to)
    {
        return QJsonObject {
            {"type", "FRIEND_REQUEST_SENT_RESPONSE"},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"to", to}
            }}
        };
    }

    inline QJsonObject cancelFriendRequestResponse(bool success, const QString &message, const QString &to)
    {
        return QJsonObject {
            {"type", "FRIEND_REQUEST_CANCEL_RESPONSE"},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"to", to}
            }}
        };
    }

    inline QJsonObject getFriendListResponse(bool success, const QJsonArray &friendArray)
    {
        return QJsonObject {
            {"type", "FETCH_FRIEND_LIST_RESPONSE"},
            {"payload", QJsonObject{
                {"success", success},
                {"friends", friendArray}
            }}
        };
    }

    inline QJsonObject getSendRequestListResponse(bool success, const QJsonArray &sentArray)
    {
        return QJsonObject {
            {"type", "FETCH_SENT_REQUESTS_RESPONSE"},
            {"payload", QJsonObject{
                {"success", success},
                {"sent", sentArray}
            }}
        };
    }

    inline QJsonObject getPendingRequestsResponse(bool success, const QJsonArray &requestArray)
    {
        return QJsonObject {
            {"type", "FETCH_RECEIVED_REQUESTS_RESPONSE"},
            {"timestamp", static_cast<int>(QDateTime::currentSecsSinceEpoch())},
            {"payload", QJsonObject{
                {"success", success},
                {"requests", requestArray}
            }}
        };
    }

    inline QJsonObject createFriendRequestResponse(bool success, const QString &message, const QString &from, const QString &to)
    {
        return QJsonObject {
            {"type", "FRIEND_REQUEST_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"from", from},
                {"to", to}
            }}
        };
    }

    inline QJsonObject deleteFriendResponse(bool success, const QString &message, const QString &from, const QString &to)
    {
        return QJsonObject {
            {"type", "FRIEND_DELETED_RESPONSE"},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"from", from},
                {"to", to}
            }}
        };
    }

    inline QJsonObject announceFriendDeletion(QString &from, const QString &message)
    {
        return QJsonObject { 
            {"type", "FRIEND_DELETED_NOTIFICATION"},
            {"timestamp", static_cast<int>(QDateTime::currentSecsSinceEpoch())},
            {"payload", QJsonObject{
                {"from", from},
                {"message", message}
            }}
        };
    }
    
}

#endif // REQUEST_FACTORY_H