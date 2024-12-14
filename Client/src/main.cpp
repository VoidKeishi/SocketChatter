#include <QGuiApplication>
#include <QtCore>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QSqlDatabase>
#include <QSqlError>
#include "models/ContactModel.h"
#include "models/ConversationModel.h"
#include "controllers/ControllerManager.h"
#include "controllers/network/NetworkController.h"

static void connectToDatabase()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
    }

    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDir.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));

    // Ensure that we have a writable location on all devices.
    const QString fileName = writeDir.absolutePath() + "/chat-database.sqlite3";
    // When using the SQLite driver, open() will create the SQLite database if it doesn't exist.
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    if (argc < 3) {
        qFatal("Usage: %s <IP> <PORT>", argv[0]);
        return -1;
    }

    QString ip = argv[1];
    int port = QString(argv[2]).toInt();
    NetworkController::instance()->connectToServer(ip, port);

    ControllerManager controllerManager;
    controllerManager.initControllers();
    connectToDatabase();

    qmlRegisterType<SqlContactModel>("Client", 1, 0, "SqlContactModel");
    qmlRegisterType<SqlConversationModel>("Client", 1, 0, "SqlConversationModel");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("loginController", controllerManager.loginController);
    engine.rootContext()->setContextProperty("registerController", controllerManager.registerController);
    engine.rootContext()->setContextProperty("manageContactsController", controllerManager.manageContactsController);
    engine.load(QUrl(QStringLiteral("qrc:/src/views/AuthWindow.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}