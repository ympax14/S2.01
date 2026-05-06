#ifndef DESERIALIZER_HPP
#define DESERIALIZER_HPP

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>

namespace Deserializer {
    bool isQuint16(const QJsonValue& value) {
        if (value.isDouble()) {
            double num = value.toDouble();

            // Check if it is a whole number and within quint16 range [0, 65535]
            return num == static_cast<int>(num) && num >= 0 && num <= 65535;
        } else return false;
    }

    bool isQuint8(const QJsonValue& value) {
        if (value.isDouble()) {
            double num = value.toDouble();

            // Check if it is a whole number and within quint8 range [0, 255]
            return num == static_cast<int>(num) && num >= 0 && num <= 255;
        } else return false;
    }

    template<typename T> bool isValidEnumValue(QJsonValue value) {
        if (value.isUndefined() || value.isNull() || !value.isString())
            return false;

        const QString str = value.toString();
        if (str.isEmpty())
            return false;

        int val = QMetaEnum::fromType<T>().keyToValue(str.toUtf8().constData());

        return val != -1;
    }

    template<typename T> T parseEnumValue(QJsonValue value) {
        if (value.isUndefined() || value.isNull() || !value.isString())
            return false;

        const QString str = value.toString();
        if (str.isEmpty())
            return false;

        int val = QMetaEnum::fromType<T>().keyToValue(str.toUtf8().constData());

        if (val != -1)
            return static_cast<T>(val);
        else
            throw std::invalid_argument("Cette valeur n'appartient pas à l'énumération !");
    }

    void JSON(QString path) {
        QFile file = QFile(path);
        if (!file.open(QIODevice::ReadOnly)) {
            throw std::invalid_argument("Invalid input path to deserialize.");
        }

        QByteArray byteArray = file.readAll();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
        QJsonObject jsonObj  = jsonDocument.object();
    }
}

#endif // DESERIALIZER_HPP
