#include "ClientThread.h"

ClientThread::ClientThread(QTcpSocket* socket, QObject* parent)
    : QThread(parent), clientSocket(socket) {
}

void ClientThread::run() {
    ClientHandler* handler = new ClientHandler(clientSocket);
    exec(); // Starts the event loop for this thread
}