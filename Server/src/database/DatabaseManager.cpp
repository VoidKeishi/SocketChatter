#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>


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
        qCritical() << "Cannot open database:" << db.lastError().text();
        return false;
    }

    QFile schemaFile(":/src/database/schema.sql");
    if (!schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open schema file";
        return false;
    }

    QString schema = schemaFile.readAll();
    QStringList statements = schema.split(';', Qt::SkipEmptyParts);

    db.transaction();
    for (const QString& statement : statements) {
        QSqlQuery query(db);
        if (!query.exec(statement.trimmed())) {
            qCritical() << "Failed to execute:" << statement;
            qCritical() << "Error:" << query.lastError().text();
            db.rollback();
            return false;
        }
    }
    db.commit();
    
    qDebug() << "Database initialized successfully";
    return true;
}