#include "DatabaseManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager* DatabaseManager::instance() {
    if (!m_instance) {
        m_instance = new DatabaseManager();
    }
    return m_instance;
}

DatabaseManager::DatabaseManager(QObject* parent) : QObject(parent) {
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dataPath + "/chat.db");
}

bool DatabaseManager::init() {
    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return false;
    }
    
    return initTables();
}

bool DatabaseManager::initTables() {
    QFile schemaFile(":/database/schema.sql");
    if (!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open schema file";
        return false;
    }

    QString schema = schemaFile.readAll();
    return executeSchema(schema);
}

bool DatabaseManager::executeSchema(const QString& schema) {
    db.transaction();
    
    QStringList statements = schema.split(';', Qt::SkipEmptyParts);
    for (const QString& statement : statements) {
        QSqlQuery query(db);
        if (!query.exec(statement.trimmed())) {
            qCritical() << "Failed to execute:" << statement.trimmed();
            qCritical() << "Error:" << query.lastError().text();
            db.rollback();
            return false;
        }
    }
    
    return db.commit();
}