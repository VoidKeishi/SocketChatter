#pragma once
#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    static DatabaseManager* instance();
    bool init();
    QSqlDatabase& database() { return db; }

private:
    explicit DatabaseManager(QObject* parent = nullptr);
    static DatabaseManager* m_instance;
    QSqlDatabase db;
    
    bool initTables();
    bool executeSchema(const QString& schema);
};