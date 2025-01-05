#pragma once

#include <QSqlDatabase>
#include <QObject>

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    static DatabaseManager* instance();
    QSqlDatabase& database() { return db; }
    bool init();

private:
    explicit DatabaseManager(QObject* parent = nullptr);
    static DatabaseManager* m_instance;
    QSqlDatabase db;
};
