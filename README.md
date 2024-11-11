# LAN-Based Real-Time Messaging Application

## Description

This application is a LAN-based real-time messaging platform that allows users to send and receive text messages, images, audio, and video within a local network environment. Users can create private and group chats, manage friend lists, and view chat histories. The application is designed for speed and reliability, utilizing TCP/IP socket connections over LAN or WLAN.

## Features

- **User Registration & Login**
  - Users can create an account and securely log in to the application.
- **Friends List Management**
  - View friends' online or offline status in real-time.
  - Send and accept friend requests.
- **Group Chats**
  - Create group chats and invite friends to join.
  - Accept group chat invitations.
  - Leave group chats at any time.
- **Real-time Messaging**
  - Send and receive text messages.
  - Share images, audio files, and video files.
  - Support for both private and group conversations.
- **Message History**
  - View chat history for individual and group conversations.
- **Logging**
  - Server logs all incoming and outgoing messages for monitoring and debugging.

## Requirements

- **Client-Server Connection via Socket**
  - Establish a reliable connection between clients and the server using TCP sockets.
- **LAN/WLAN Communication**
  - Clients and server communicate over known private IP addresses within the same local network.
- **Server-Client Model**
  - Centralized server manages communication between clients.
- **Message Logging on the Server**
  - All messages are logged on the server for record-keeping.

## Technologies

- **Frontend**: Qt QML for a cross-platform graphical user interface.
- **Backend**: C++ with the Qt framework for robust application logic.
- **Database**: SQLite for local data storage.
- **Networking**: TCP/IP sockets for efficient network communication.

## Architecture

The application follows a client-server model, where multiple clients connect to a central server over LAN or WLAN. The server handles user authentication, message routing, and logging, ensuring secure and efficient communication.

## Scoring Breakdown

1. **Client-Server Connection via Socket** (2 points)
   - Implement reliable socket connections between clients and the server.
2. **Message Logging on the Server** (1 point)
   - Log all incoming and outgoing messages for auditing purposes.
3. **User Registration and Login** (2 points)
   - Enable account creation and secure user authentication.
4. **View Friends List (Online/Offline Status)** (2 points)
   - Display real-time online/offline status of friends.
5. **Friend Requests and Acceptance** (2 points)
   - Allow users to send and accept friend requests.
6. **Group Chat Management** (3 points)
   - Support creating groups, inviting friends, and managing group membership.
7. **Messaging (Text, Image, Audio, Video)** (4 points)
   - Facilitate sending and receiving various types of messages in chats.
8. **Message History Viewing** (2 points)
   - Provide access to view past conversations in chat history.

## Usage

1. **Account Registration**
   - Open the client application.
   - Register a new account with a unique username and password.
2. **Login**
   - Log in using your credentials to access the main interface.
3. **Managing Friends**
   - Search for other users and send friend requests.
   - Accept incoming friend requests from others.
   - View your friends list with their online/offline status.
4. **Creating and Joining Group Chats**
   - Create a new group chat and invite friends.
   - Accept invitations to join existing group chats.
5. **Messaging**
   - Start private or group conversations.
   - Send text messages, images, audio clips, and videos.
6. **Viewing Message History**
   - Access the history of your conversations from the chat interface.

## Contributors

- **Pham Phan Anh** - 20210039
- **Dao Sy Phuc** - 20215117
