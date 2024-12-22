#ifndef HASHUTILITY_H
#define HASHUTILITY_H

#include <QString>
#include <QCryptographicHash>

namespace HashUtility {
    inline QString hashPassword(const QString &password) {
        QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
        return QString(hashed.toHex());
    }
}

#endif // HASHUTILITY_H