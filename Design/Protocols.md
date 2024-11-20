# Protocols
Here are the protocols for the client and server communication.
- [User Authentication](#user-authentication)
  - [Registration](#registration)
  - [Login](#login)
- [Friends Management](#friends-management)
    - [Friend Request](#friend-request)
    - [Friend Removal](#friend-removal)
- [Group Chat Management](#group-chat-management)
    - [Group Chat Creation](#group-chat-creation)
    - [Group Chat Invitation](#group-chat-invitation)
    - [Leave Group Chat](#leave-group-chat)
    - [Group Chat Removal](#group-chat-removal)
    - [Leader Change](#leader-change)
- [Messaging](#messaging)
    - [Text Message](#text-message)
    - [Media Message](#media-message)
- [Presence Management](#presence-management)
    - [User Status Update](#user-status-update)
    - [User Logout](#user-logout)
- [Contact History](#contact-history)
# User Authentication
## Registration
- Flow: 
  1. Client sends a `REGISTER_REQUEST` message to the server.
  2. Server processes the request and sends a `REGISTER_RESPONSE` message back to the client.
### Message Type: REGISTER_REQUEST
```json
{
  "type": "REGISTER_REQUEST",
  "timestamp": 1630000000,
  "payload": {
    "username": "user123",
    "passwordHash": "hashed_password"
  }
}
```
### Message Type: REGISTER_RESPONSE
```json
{
  "type": "REGISTER_RESPONSE",
  "timestamp": 1630000001,
  "payload": {
    "success": true,
    "code": 0
  }
}
```
| Code | Description                  |
|------|------------------------------|
|   0  | Success                      |
| 1001 | Authentication failed        |
| 1002 | User already exists          |
| 1003 | Server error                 |

## Login
- Flow: 
  1. Client sends a `LOGIN_REQUEST` message to the server.
  2. Server processes the request and sends a `LOGIN_RESPONSE` message back to the client.
### Message Type: LOGIN_REQUEST
```json
{
  "type": "LOGIN_REQUEST",
  "timestamp": 1630000002,
  "payload": {
    "username": "user123",
    "passwordHash": "hashed_password"
  }
}
```
### Message Type: LOGIN_RESPONSE
```json
{
    "type": "LOGIN_RESPONSE",
    "timestamp": 1630000003,
    "payload": {
        "success": true,
        "code": 0 // Success
    }
}
```

| Code | Description                  |
|------|------------------------------|
|   0  | Success                      |
| 2001 | Invalid credentials          |
| 2002 | User already logged in       |
| 2003 | User not found               |
| 2004 | Server error                 |
# Friends Management
## Friend Request
- Flow: 
  1. Sender client sends a `FRIEND_REQUEST` message to the server.
  2. Server sends a `FRIEND_REQUEST_ACK` message to the sender client.
  3. Server checks if the receiver is online. 
    - If online: server sends a `FRIEND_REQUEST` message to the receiver client. 
    - If offline: the request is stored for later delivery and will be triggered when the receiver comes online.
  4. Receiver client receives the `FRIEND_REQUEST` message and responds with a `FRIEND_REQUEST_RESPONSE` message.
  5. Server forward the `FRIEND_REQUEST_RESPONSE` message to the sender client.
### Message Type: FRIEND_REQUEST
```json
{
  "type": "FRIEND_REQUEST",
  "timestamp": 1630000004,
  "payload": {
    "sender": "user123",
    "receiver": "user456"
  }
}
```
### Message Type: FRIEND_REQUEST_ACK
```json
{
  "type": "FRIEND_REQUEST_ACK",
  "timestamp": 1630000004,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
    "code": 0 // Acknowledged
  }
}
```
| Code  | Description                  |
|-------|------------------------------|
| 0     | Success                      |
| 3001  | User not found               |
| 3002  | Server error                 |
### Message Type: FRIEND_REQUEST_RESPONSE
```json
{
  "type": "FRIEND_REQUEST_RESPONSE",
  "timestamp": 1630000005,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
    "code": 0 // Request accepted
  }
}
```
| Code  | Description                  |
|-------|------------------------------|
| 0     | Success                      |
| 3004  | Request rejected             |
## Friend Removal
- Flow: 
  1. Client sends a `FRIEND_REMOVE` message to the server.
  2. Server processes the request and sends a `FRIEND_REMOVE_ACK` message back to the client.
  3. Server sends a `FRIEND_REMOVE` message to the friend client.
### Message Type: FRIEND_REMOVE
```json
{
  "type": "FRIEND_REMOVE",
  "timestamp": 1630000006,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
  }
}
```
### Message Type: FRIEND_REMOVE_ACK
```json
{
  "type": "FRIEND_REMOVE_ACK",
  "timestamp": 1630000006,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
    "code": 0 // Acknowledged
  }
}
```
# Group Chat Management
## Group Chat Creation
- Flow: 
  1. Client sends a `GROUP_CREATE` message to the server.
  2. If all the users are friends with the sender, the server creates the group and sends a `GROUP_CREATE_ACK` message back to the sender client.
  If any user is not a friend, the server sends a `GROUP_CREATE_ACK` message with an error code.
  3. The group is created and added to the sender's group list.
  4. All invited users receive a `GROUP_INVITE` message. Begin the [Group Chat Invitation](#group-chat-invitation) flow.
### Message Type: GROUP_CREATE
```json
{
  "type": "GROUP_CREATE",
  "timestamp": 1630000007,
  "payload": {
    "sender": "user123",
    "members": ["user456", "user789"],
    "name": "Group Chat 1"
  }
}
```
### Message Type: GROUP_CREATE_ACK
#### Success
```json
{
  "type": "GROUP_CREATE_ACK",
  "timestamp": 1630000008,
  "payload": {
    "sender": "user123",
    "members": ["user456", "user789"],
    "name": "Group Chat 1",
    "code": 0 // Success
  }
}
```
#### Failure
```json
{
  "type": "GROUP_CREATE_ACK",
  "timestamp": 1630000008,
  "payload": {
    "sender": "user123",
    "not-friends": ["user789"],
    "name": "Group Chat 1",
    "code": 4001 // Not all members are friends
  }
}
```
## Group Chat Invitation
- Flow: 
  1. Client receives a `GROUP_INVITE` message.
  2. Client responds with a `GROUP_INVITE_RESPONSE` message.
  3. Server forwards the `GROUP_INVITE_RESPONSE` message to the sender client.
### Message Type: GROUP_INVITE
```json
{
  "type": "GROUP_INVITE",
  "timestamp": 1630000009,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
    "group": "Group Chat 1"
  }
}
```
### Message Type: GROUP_INVITE_RESPONSE
#### Accept
```json
{
  "type": "GROUP_INVITE_RESPONSE",
  "timestamp": 1630000010,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
    "group": "Group Chat 1",
    "code": 0 // Invitation accepted
  }
}
```
#### Reject
```json
{
  "type": "GROUP_INVITE_RESPONSE",
  "timestamp": 1630000010,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
    "group": "Group Chat 1",
    "code": 5001 // Invitation rejected
  }
}
```
## Leave Group Chat
- Flow: 
  1. Client sends a `GROUP_LEAVE` message to the server.
  2. Server processes the request and sends a `GROUP_LEAVE_ACK` message back to the client.
  3. Server sends a `GROUP_LEAVE` message to all group members.
### Message Type: GROUP_LEAVE
```json
{
  "type": "GROUP_LEAVE",
  "timestamp": 1630000011,
  "payload": {
    "sender": "user123",
    "group": "Group Chat 1"
  }
}
```
### Message Type: GROUP_LEAVE_ACK
```json
{
  "type": "GROUP_LEAVE_ACK",
  "timestamp": 1630000011,
  "payload": {
    "sender": "user123",
    "group": "Group Chat 1",
    "code": 0
  }
}
```
## Group Chat Removal
- Flow: 
  1. Client sends a `GROUP_REMOVE` message to the server.
  2. Server processes the request and sends a `GROUP_REMOVE_ACK` message back to the client.
  3. Server sends a `GROUP_REMOVE` message to all group members.
### Message Type: GROUP_REMOVE
```json
{
  "type": "GROUP_REMOVE",
  "timestamp": 1630000012,
  "payload": {
    "sender": "user123",
    "group": "Group Chat 1"
  }
}
```
### Message Type: GROUP_REMOVE_ACK
```json
{
  "type": "GROUP_REMOVE_ACK",
  "timestamp": 1630000012,
  "payload": {
    "sender": "user123",
    "group": "Group Chat 1",
    "code": 0 // Acknowledged
  }
}
```
## Leader Change
- Flow: 
  1. Client sends a `GROUP_LEADER_CHANGE` message to the server.
  2. Server processes the request and sends a `GROUP_LEADER_CHANGE_ACK` message back to the client.
  3. Server sends a `GROUP_LEADER_CHANGE` message to all group members.
### Message Type: GROUP_LEADER_CHANGE
```json
{
  "type": "GROUP_LEADER_CHANGE",
  "timestamp": 1630000013,
  "payload": {
    "sender": "user123",
    "group": "Group Chat 1",
    "newLeader": "user456
   }
}
```
### Message Type: GROUP_LEADER_CHANGE_ACK
```json
{
  "type": "GROUP_LEADER_CHANGE_ACK",
  "timestamp": 1630000013,
  "payload": {
    "sender": "user123",
    "group": "Group Chat 1",
    "newLeader": "user456",
    "code": 0 // Acknowledged
  }
}
```
# Messaging
## Text Message
- Flow: 
  1. Client sends a `TEXT_MESSAGE` message to the server.
  2. Server processes the message and sends a `TEXT_MESSAGE` message to the receiver client.
### Message Type: TEXT_MESSAGE
```json
{
  "type": "TEXT_MESSAGE",
  "timestamp": 1630000014,
  "payload": {
    "sender": "user123",
    "receiver": "user456",
    "content": "Hello, how are you?"
  }
}
```
## Media Message
- Flow: The recipient could be a user or a group, and be offline. The server will store the message and deliver it when the recipient is online. The flow will be the same between sender - server and server - recipient. So receiver could be client or server.
  1. Sender sends a `MEDIA_MESSAGE` message to the receiver.
  2. Receiver receives the message and sends a `MEDIA_MESSAGE_ACK` message back to the sender.
3. If the media is large, the sender sends multiple `MEDIA_MESSAGE_DATA` messages to the receiver.
4. Receiver sends a `MEDIA_MESSAGE_DATA_ACK` message for each `MEDIA_MESSAGE_DATA` message received.
5. After all `MEDIA_MESSAGE_DATA` messages are received, the receiver sends a `MEDIA_MESSAGE_COMPLETE` message to the sender.
6. Sender sends a `MEDIA_MESSAGE_COMPLETE_ACK` message to the receiver.

### Message Type: MEDIA_MESSAGE
```json
{
    "type": "MEDIA_MESSAGE",
    "timestamp": 1630000015,
    "payload": {
        "sender": "user123",
        "receiver": "user456",
        "mediaId": "media123",
        "mediaType": "image/jpeg",
        "mediaSize": 10485760 // Size in bytes
    }
}
```

### Message Type: MEDIA_MESSAGE_ACK
```json
{
    "type": "MEDIA_MESSAGE_ACK",
    "timestamp": 1630000016,
    "payload": {
        "sender": "user456",
        "receiver": "user123",
        "mediaId": "media123",
        "code": 0 // Acknowledged
    }
}
```

### Message Type: MEDIA_MESSAGE_DATA
```json
{
    "type": "MEDIA_MESSAGE_DATA",
    "timestamp": 1630000017,
    "payload": {
        "sender": "user123",
        "receiver": "user456",
        "mediaId": "media123",
        "data": "base64_encoded_chunk",
        "chunkIndex": 1,
        "totalChunks": 10
    }
}
```

### Message Type: MEDIA_MESSAGE_DATA_ACK
```json
{
    "type": "MEDIA_MESSAGE_DATA_ACK",
    "timestamp": 1630000018,
    "payload": {
        "sender": "user456",
        "receiver": "user123",
        "mediaId": "media123",
        "chunkIndex": 1,
        "code": 0 // Acknowledged
    }
}
```

### Message Type: MEDIA_MESSAGE_COMPLETE
```json
{
    "type": "MEDIA_MESSAGE_COMPLETE",
    "timestamp": 1630000019,
    "payload": {
        "sender": "user456",
        "receiver": "user123",
        "mediaId": "media123",
        "code": 0 // Transfer complete
    }
}
```

### Message Type: MEDIA_MESSAGE_COMPLETE_ACK
```json
{
    "type": "MEDIA_MESSAGE_COMPLETE_ACK",
    "timestamp": 1630000020,
    "payload": {
        "sender": "user123",
        "receiver": "user456",
        "mediaId": "media123",
        "code": 0 // Acknowledged
    }
}
```
# Presence Management
## User Status Update
- Flow: 
  1. Client sends a `USER_STATUS_UPDATE` message to the server.
  2. Server processes the message and sends a `USER_STATUS_UPDATE` message to all friends of the user.
### Message Type: USER_STATUS_UPDATE
```json
{
  "type": "USER_STATUS_UPDATE",
  "timestamp": 1630000021,
  "payload": {
    "username": "user123",
    "status": "online"
  }
}
```
## User Logout
- Flow: 
  1. Client sends a `USER_LOGOUT` message to the server.
  2. Server processes the message and sends a `USER_LOGOUT` message to all friends of the user.
### Message Type: USER_LOGOUT
```json
{
  "type": "USER_LOGOUT",
  "timestamp": 1630000022,
  "payload": {
    "username": "user123"
  }
}
```
# Message History Sync
- Flow:
  1. Client needs to authenticate before requesting message history.
  2. Client sends a `MESSAGE_HISTORY_REQUEST` message to the server.
  3. Server processes the request and sends a `MESSAGE_HISTORY_RESPONSE` message back to the client.
    - If the request is valid and history exists, the server sends a `MESSAGE_HISTORY_BEGIN` message to the client.
    - If the request is invalid or no history exists, the server sends a `MESSAGE_HISTORY_RESPONSE` message with an error code.
  4. Upon receiving the `MESSAGE_HISTORY_BEGIN` message, the client acknowledges it.
  5. Server begins sending `MESSAGE_HISTORY_DATA` messages, each containing a chunk of the message history.
  6. Client acknowledges each `MESSAGE_HISTORY_DATA` message.
  7. After all `MESSAGE_HISTORY_DATA` messages are sent, the server sends a `MESSAGE_HISTORY_COMPLETE` message to the client.
  8. Client acknowledges the `MESSAGE_HISTORY_COMPLETE` message.

### Message Type: MESSAGE_HISTORY_REQUEST
```json
{
  "type": "MESSAGE_HISTORY_REQUEST",
  "timestamp": 1630000023,
  "payload": {
  "username": "user123"
  }
}
```

### Message Type: MESSAGE_HISTORY_RESPONSE
#### Success
```json
{
  "type": "MESSAGE_HISTORY_RESPONSE",
  "timestamp": 1630000024,
  "payload": {
  "username": "user123",
  "code": 0 // Success
  }
}
```

#### Failure
```json
{
  "type": "MESSAGE_HISTORY_RESPONSE",
  "timestamp": 1630000024,
  "payload": {
  "username": "user123",
  "code": 6001 // No history found
  }
}
```

### Message Type: MESSAGE_HISTORY_BEGIN
```json
{
  "type": "MESSAGE_HISTORY_BEGIN",
  "timestamp": 1630000025,
  "payload": {
  "username": "user123",
  "code": 0 // Handshake initiated
  }
}
```

### Message Type: MESSAGE_HISTORY_DATA
```json
{
  "type": "MESSAGE_HISTORY_DATA",
  "timestamp": 1630000026,
  "payload": {
  "username": "user123",
  "messages": [
    {
    "timestamp": 1630000001,
    "sender": "user123",
    "receiver": "user456",
    "content": "Hello!"
    },
    {
    "timestamp": 1630000002,
    "sender": "user456",
    "receiver": "user123",
    "content": "Hi there!"
    }
  ]
  }
}
```

### Message Type: MESSAGE_HISTORY_COMPLETE
```json
{
  "type": "MESSAGE_HISTORY_COMPLETE",
  "timestamp": 1630000027,
  "payload": {
  "username": "user123",
  "code": 0 // Transfer complete
  }
}
```



