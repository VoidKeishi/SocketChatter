#include <QSqlError>
#include <QSqlQuery>
#include <QFile>

#include "DatabaseManager.h"
#include "../utils/Logger.h"


DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager::DatabaseManager(QObject* parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat.db");
}

DatabaseManager* DatabaseManager::instance() {
    if (!m_instance) {
        m_instance = new DatabaseManager();
    }
    return m_instance;
}

bool DatabaseManager::init() {
    if (!db.open()) {
        Logger::error("Cannot open database: " + db.lastError().text());
        return false;
    }

    QFile schemaFile(":/src/database/schema.sql");
    if (!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Logger::error("Cannot open schema file");
        return false;
    }

    QString schema = schemaFile.readAll();
    QStringList statements = schema.split(';', Qt::SkipEmptyParts);

    db.transaction();
    for (const QString& statement : statements) {
        QSqlQuery query(db);
        if (!query.exec(statement.trimmed())) {
            Logger::error("Failed to execute: " + statement);
            Logger::error("Error:" + query.lastError().text());            
            db.rollback();
            return false;
        }
    }
    db.commit();
    
    Logger::debug("Database initialized successfully");
    return true;
}