#ifndef SONG_HPP
#define SONG_HPP

#include "../utils/Deserializer.hpp"

#include <QtTypes>
#include <QString>
#include <QJsonObject>
#include <functional>
#include <map>

class Song {
private:
    quint16 index = 0;
    QString title = "";
    quint16 duration = 0;
    double rating = 0.0;

public:
    Song() {};
    ~Song() = default;

    inline void setIndex(quint16 _index) {
        this->index = _index;
    }

    inline void setTitle(QString _title) {
        this->title = _title;
    }

    inline void setDuration(quint16 _duration) {
        this->duration = _duration;
    }

    inline void setRating(double _rating) {
        this->rating = _rating;
    }

    inline quint16 getIndex() const {
        return this->index;
    }

    inline QString getTitle() const {
        return this->title;
    }

    inline quint16 getDuration() const {
        return this->duration;
    }

    inline double getRating() const {
        return this->rating;
    }

    bool verifyOutputIntegrity() const {
        if (this->title.isNull() || this->title.isEmpty())
            return false;

        if (this->rating > 1.0 || this->rating < 0.0)
            return false;

        return true;
    }

    QJsonObject toJson() const {
        if (!this->verifyOutputIntegrity())
            throw std::invalid_argument("Invalid Song to serialize !");

        QJsonObject obj;

        obj["index"] = this->index;
        obj["title"] = this->title;
        obj["duration"] = this->duration;
        obj["rating"] = this->rating;

        return obj;
    }

    static bool verifyInputIntegrity(const QJsonObject &obj) {
        static std::map<QString, std::function<bool(QJsonValue)>> requiredKeys = {
            {"index", [](QJsonValue value) -> bool {
                return Deserializer::isQuint8(value);
            }},
            {"title", [](QJsonValue value) -> bool {
                return value.isString();
            }},
            {"duration", [](QJsonValue value) -> bool {
                return Deserializer::isQuint16(value);
            }},
            {"rating", [](QJsonValue value) -> bool {
                return value.isDouble();
            }}
        };

        for (const auto& pair : requiredKeys)
            if (!obj.contains(pair.first) || !pair.second(obj[pair.first]))
                return false;

        return true;
    }

    static Song fromJson(const QJsonObject& obj) {
        if (!Song::verifyInputIntegrity(obj))
            throw std::invalid_argument("Invalid Song to deserialize !");

        quint8 index = obj["index"].toInt();
        QString title = obj["title"].toString();
        quint16 duration = obj["duration"].toInt();
        double rating = obj["rating"].toDouble();

        Song song;
        song.setIndex(index);
        song.setTitle(title);
        song.setDuration(duration);
        song.setRating(rating);

        return song;
    }
};

#endif // SONG_HPP
