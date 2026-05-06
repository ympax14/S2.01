#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <QJsonObject>
#include <QMetaEnum>
#include <QFile>

class Serializer {
public:
    template<typename T> static const char* serializeEnumValue(T value) {
        return QMetaEnum::fromType<T>().valueToKey(value);
    }

    static void saveData(QString path, const QJsonObject& obj) {
        if (path.isNull() || path.isEmpty())
            throw std::invalid_argument("Specified file path is invalid.");

        QJsonDocument doc(obj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

        QFile file(path);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonData);
            file.close();
        } else
            throw std::runtime_error("Specified file is busy.");
    }
};

#endif // SERIALIZER_HPP
