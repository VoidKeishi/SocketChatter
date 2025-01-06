#pragma once

#include <QJsonObject>
#include <QString>
#include <QDateTime>
#include <QJsonDocument>

namespace RequestFactory
{
    // Auth requests
    inline QByteArray createLoginRequest(const QString &username, const QString &password)
    {
        QJsonObject request = {
            {"type", "LOGIN_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"username", username},
                            {"password", password}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createRegisterRequest(const QString &username, const QString &password)
    {
        QJsonObject request = {
            {"type", "REGISTER_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"username", username},
                            {"password", password}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    // Contacts requests
    inline QByteArray createSendFriendRequest(const QString &from, const QString &to)
    {
        QJsonObject request = {
            {"type", "SEND_FRIEND_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"from", from},
                            {"to", to}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createCancelFriendRequest(const QString &from, const QString &to)
    {
        QJsonObject request = {
            {"type", "CANCEL_FRIEND_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"from", from},
                            {"to", to}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createResponseFriendRequest(const QString &from, const QString &to, bool accept)
    {
        QJsonObject request = {
            {"type", "RESPOND_TO_FRIEND_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"from", from},
                            {"to", to},
                            {"accept", accept}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createDeleteFriendRequest(const QString &from, const QString &to)
    {
        QJsonObject request = {
            {"type", "DELETE_FRIEND_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"from", from},
                            {"to", to}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createFetchFriendsListRequest(const QString& username)
    {
        QJsonObject request = {
            {"type", "FETCH_FRIEND_LIST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"username", username}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createFetchSentRequestsRequest(const QString& username)
    {
        QJsonObject request = {
            {"type", "FETCH_SENT_REQUESTS"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"username", username}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createFetchReceivedRequestsRequest(const QString& username)
    {
        QJsonObject request = {
            {"type", "FETCH_RECEIVED_REQUESTS"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"username", username}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    // Messages requests
    inline QByteArray createSendMessageRequest(const QString &sender, const QString &receiver, const QString &content)
    {
        QJsonObject request = {
            {"type", "SEND_MESSAGE_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"receiver", receiver},
                {"content", content}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createFetchMessagesRequest(const QString &sender, const QString &receiver)
    {
        QJsonObject request = {
            {"type", "FETCH_MESSAGES_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender}, 
                {"receiver", receiver}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    // Groups requests
    inline QByteArray createGroupCreateRequest(const QString& sender, const QString& groupName, const QString& groupDetails)
    {
        QJsonObject request = {
            {"type", "GROUP_CREATE_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"groupName", groupName}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createGroupFetchRequest(const QString& sender)
    {
        QJsonObject request = {
            {"type", "GROUP_FETCH_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createGroupDeleteRequest(const QString& sender, const QString& groupName)
    {
        QJsonObject request = {
            {"type", "GROUP_DELETE_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"groupName", groupName}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createGroupInviteRequest(const QString& sender, const QString& groupName, const QString& invitee)
    {
        QJsonObject request = {
            {"type", "GROUP_INVITE_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"groupName", groupName},
                {"invitee", invitee}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createGroupInviteResponseRequest(const QString& sender, const QString& groupName, bool accept)
    {
        QJsonObject request = {
            {"type", "GROUP_INVITE_RESPONSE_REQUEST"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                {"sender", sender},
                {"groupName", groupName},
                {"accept", accept}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }
}
