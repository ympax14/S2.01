#ifndef DESERIALIZER_HPP
#define DESERIALIZER_HPP

#include <QFile>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include <QDomDocument>

class Deserializer {
public:
    static bool isQuint16(const QJsonValue& value) {
        if (value.isDouble()) {
            double num = value.toDouble();

            // Check if it is a whole number and within quint16 range [0, 65535]
            return num == static_cast<int>(num) && num >= 0 && num <= 65535;
        } else return false;
    }

    static bool isQuint8(const QJsonValue& value) {
        if (value.isDouble()) {
            double num = value.toDouble();

            // Check if it is a whole number and within quint8 range [0, 255]
            return num == static_cast<int>(num) && num >= 0 && num <= 255;
        } else return false;
    }

    template<typename T> static bool isValidEnumValue(QJsonValue value) {
        if (value.isUndefined() || value.isNull() || !value.isString())
            return false;

        const QString str = value.toString();
        if (str.isEmpty())
            return false;

        int val = QMetaEnum::fromType<T>().keyToValue(str.toUtf8().constData());

        return val != -1;
    }

    template<typename T> static T parseEnumValue(QJsonValue value) {
        if (value.isUndefined() || value.isNull() || !value.isString())
            throw std::invalid_argument("Specified value is invalid");

        const QString str = value.toString();
        if (str.isEmpty())
            throw std::invalid_argument("Specified value is null");

        int val = QMetaEnum::fromType<T>().keyToValue(str.toUtf8().constData());

        if (val != -1)
            return static_cast<T>(val);
        else
            throw std::invalid_argument("The value isn't a part of the specified enum.");
    }

    template<typename T> static T parseEnumValue(const QString& value) {
        if (value.isEmpty())
            throw std::invalid_argument("Specified value is null");

        int val = QMetaEnum::fromType<T>().keyToValue(value.toUtf8().constData());
        if (val != -1)
            return static_cast<T>(val);
        else
            throw std::invalid_argument("The value isn't a part of the specified enum.");
    }

    static QJsonObject loadJson(QString path) {
        if (path.isNull() || path.isEmpty())
            throw std::invalid_argument("Specified file path is invalid.");

        QFile file = QFile(path);
        if (!file.open(QIODevice::ReadOnly))
            throw std::invalid_argument("Invalid file path to deserialize.");

        QByteArray byteArray = file.readAll();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
        return jsonDocument.object();
    }

    static QDomDocument loadXml(QString path) {
        if (path.isNull() || path.isEmpty())
            throw std::invalid_argument("Specified file path is invalid.");

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly))
            throw std::invalid_argument("Invalid file path to deserialize.");

        QDomDocument doc;
        doc.setContent(&file);
        file.close();

        return doc;
    }
};

#endif // DESERIALIZER_HPP
