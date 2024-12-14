# ChattingApp Code Structure

## Overview
This document outlines the code structure for the ChattingApp project. The application will be implemented using C++, Qt, and QML, following Object-Oriented Programming (OOP) principles. The project consists of two main components: the client and the server.

## Project Structure
```
ChattingApp/
├── Client/
│   ├── src/
│   │   ├── controllers/
│   │   │   ├── ChatController.h
│   │   │   ├── ChatController.cpp
│   │   │   ├── FriendController.h
│   │   │   ├── FriendController.cpp
│   │   │   ├── GroupController.h
│   │   │   ├── GroupController.cpp
│   │   │   ├── LoginController.h
│   │   │   └── LoginController.cpp
│   │   ├── models/
│   │   │   ├── GroupModel.h
│   │   │   ├── GroupModel.cpp
│   │   │   ├── MessageModel.h
│   │   │   ├── MessageModel.cpp
│   │   │   ├── UserModel.h
│   │   │   └── UserModel.cpp
│   │   ├── views/
│   │   │   ├── ChatView.h
│   │   │   ├── ChatView.cpp
│   │   │   ├── FriendListView.h
│   │   │   ├── FriendListView.cpp
│   │   │   ├── GroupView.h
│   │   │   ├── GroupView.cpp
│   │   │   ├── LoginView.h
│   │   │   └── LoginView.cpp
│   │   ├── network/
│   │   │   ├── NetworkManager.h
│   │   │   └── NetworkManager.cpp
│   │   ├── utils/
│   │   │   ├── JsonParser.h
│   │   │   └── JsonParser.cpp
│   │   ├── resources/
│   │   │   └── (resource files such as images, icons)
│   │   └── main.cpp
│   └── CMakeLists.txt
└── Server/
    ├── src/
    │   ├── controllers/
    │   │   ├── ClientHandler.h
    │   │   ├── ClientHandler.cpp
    │   │   ├── ServerController.h
    │   │   └── ServerController.cpp
    │   ├── models/
    │   │   ├── GroupModel.h
    │   │   ├── GroupModel.cpp
    │   │   ├── MessageModel.h
    │   │   ├── MessageModel.cpp
    │   │   ├── UserModel.h
    │   │   └── UserModel.cpp
    │   ├── network/
    │   │   ├── ProtocolHelper.h
    │   │   └── ProtocolHelper.cpp
    │   ├── database/
    │   │   ├── DatabaseManager.h
    │   │   └── DatabaseManager.cpp
    │   ├── utils/
    │   │   ├── Logger.h
    │   │   └── Logger.cpp
    │   ├── threads/
    │   │   ├── ClientThread.h
    │   │   └── ClientThread.cpp
    │   └── main.cpp
    └── CMakeLists.txt
```