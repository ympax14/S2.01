#ifndef SONG_HPP
#define SONG_HPP

#include "../utils/Deserializer.hpp"

#include <QtTypes>
#include <QString>
#include <QJsonObject>
#include <functional>
#include <map>
#include <QDomDocument>
#include <QDomElement>

class Song {
private:
    quint16 index;
    QString title;
    quint16 duration;
    double rating;

public:
    Song() : index(0), title("New Song"), duration(0), rating(0.0) {};
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

    QString getFormattedDuration() const {
        int minutes = this->getDuration() / 60;
        int seconds = this->getDuration() % 60;
        return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    }

    inline double getRating() const {
        return this->rating;
    }

    bool verifyOutputIntegrity() const {
        if (this->title.isNull() || this->title.isEmpty())
            return false;

        if (this->rating > 5.0 || this->rating < 0.0)
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

    QDomElement toXml(QDomDocument& doc) const {
        if (!this->verifyOutputIntegrity())
            throw std::invalid_argument("Invalid Song to serialize !");

        // Créer la balise <song> qui contiendra tout les champs suivants
        QDomElement song = doc.createElement("song");

        // <index>valeur</index> et l'ajoute dans <song>
        QDomElement indexElem = doc.createElement("index");
        indexElem.appendChild(doc.createTextNode(QString::number(this->index)));
        song.appendChild(indexElem);

        QDomElement titleElem = doc.createElement("title");
        titleElem.appendChild(doc.createTextNode(this->title));
        song.appendChild(titleElem);

        QDomElement durationElem = doc.createElement("duration");
        durationElem.appendChild(doc.createTextNode(QString::number(this->duration)));
        song.appendChild(durationElem);

        QDomElement ratingElem = doc.createElement("rating");
        ratingElem.appendChild(doc.createTextNode(QString::number(this->rating)));
        song.appendChild(ratingElem);

        return song;

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

    // fonction qui reconstruit un objet Song C++ à partir d'un fichier XML
    static Song fromXml(const QDomElement&  elem) {
        if (elem.isNull())
            throw std::invalid_argument("Invalid Song to deserialize !");

        Song song;

        // firstChildElement("index") cherche la balise <index>
        // .text() récupère donc le texte à l'intérieur
        // et .toInt() convertit ce texte en nombre entier

        song.setIndex(elem.firstChildElement("index").text().toInt());
        song.setTitle(elem.firstChildElement("title").text());
        song.setDuration(elem.firstChildElement("duration").text().toInt());
        song.setRating(elem.firstChildElement("rating").text().toDouble());

        return song;
    }
};

#endif // SONG_HPP
