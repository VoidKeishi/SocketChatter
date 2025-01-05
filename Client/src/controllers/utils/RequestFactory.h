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
    inline QByteArray createSendMessageRequest(const QString &contact, const QString &content)
    {
        QJsonObject request = {
            {"type", "SEND_MESSAGE_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"contact", contact},
                            {"content", content}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

    inline QByteArray createFetchMessagesRequest(const QString &contact)
    {
        QJsonObject request = {
            {"type", "FETCH_MESSAGES_RESPONSE"},
            {"timestamp", QDateTime::currentSecsSinceEpoch()},
            {"payload", QJsonObject{
                            {"contact", contact}}}};
        return QJsonDocument(request).toJson(QJsonDocument::Compact) + '\n';
    }

}
