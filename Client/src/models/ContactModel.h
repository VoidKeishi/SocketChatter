#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QQmlEngine>
#include <QSqlQueryModel>
#include <QtQml/qqmlregistration.h>

enum Roles {
    NameRole = Qt::UserRole + 1
};

class SqlContactModel : public QSqlQueryModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit SqlContactModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
};
#endif // CONTACTMODEL_H