#pragma once

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
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
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
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
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
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
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
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
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
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"requests", requestArray}
            }}
        };
    }

    inline QJsonObject createFriendRequestResponseAck(bool success, bool accept, const QString &message, const QString &from, const QString &to)
    {
        return QJsonObject {
            {"type", "FRIEND_REQUEST_RESPONSE_ACK"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"accept", accept},
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
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
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
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"from", from},
                {"message", message}
            }}
        };
    }
    
    inline QJsonObject createSendMessageResponse(const QString& sender, const QString& receiver, const QString& content) {
        return {
            {"type", "SEND_MESSAGE_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"receiver", receiver},
                {"content", content},
                {"success", true}}}};
    }

    inline QJsonObject createFetchMessagesResponse(const QString& sender, const QString& receiver, const QString& messagesString) {
        return {
            {"type", "FETCH_MESSAGES_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"receiver", receiver},
                {"messages", QJsonDocument::fromJson(messagesString.toUtf8()).array()}}}};
    }

    inline QJsonObject createGroupCreateResponse(bool success, const QString& message, const QString& groupName) {
        return QJsonObject {
            {"type", "GROUP_CREATE_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"groupName", groupName}
            }}
        };
    }

    inline QJsonObject createGroupFetchResponse(bool success, const QString& message, const QList<QPair<QString, QString>>& groups) {
        QJsonArray groupArray;
        for (const auto& group : groups) {
            QJsonObject groupObj;
            groupObj["groupName"] = group.first;
            groupObj["creator"] = group.second;
            groupArray.append(groupObj);
        }

        return QJsonObject {
            {"type", "GROUP_FETCH_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"groups", groupArray}
            }}
        };
    }

    inline QJsonObject createGroupDeleteResponse(bool success, const QString& message, const QString& groupName) {
        return QJsonObject {
            {"type", "GROUP_DELETE_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"groupName", groupName}
            }}
        };
    }

    inline QJsonObject createGroupDeletedNotification(const QString& message, const QString& groupName, const QJsonArray& members) {
        return QJsonObject {
            {"type", "GROUP_DELETED_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"message", message},
                {"groupName", groupName},
                {"members", members}
            }}
        };
    }

    inline QJsonObject createGroupInviteResponse(bool success, const QString& message, const QString& groupName, const QString& invitee) {
        return QJsonObject {
            {"type", "GROUP_INVITE_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"message", message},
                {"groupName", groupName},
                {"invitee", invitee}
            }}
        };
    }

    inline QJsonObject createGroupInviteResponseAck(bool success, bool accept, const QString& message, const QString& groupName, const QString& member) {
        return QJsonObject {
            {"type", "GROUP_INVITE_RESPONSE_ACK"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"success", success},
                {"accept", accept},
                {"message", message},
                {"groupName", groupName},
                {"member", member}
            }}
        };
    }

    inline QJsonObject createGroupInviteNotification(const QString& groupName, const QString& inviter, const QString& invitee) {
        return QJsonObject {
            {"type", "GROUP_INVITE_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"groupName", groupName},
                {"inviter", inviter},
                {"invitee", invitee}
            }}
        };
    }

    inline QJsonObject createGroupInviteResponseNotification(const QString& groupName, const QString& responder, bool accepted, const QString& creator) {
        return QJsonObject {
            {"type", "GROUP_INVITE_RESPONSE_NOTIFICATION"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"groupName", groupName},
                {"responder", responder},
                {"accepted", accepted},
                {"creator", creator}
            }}
        };
    }
}