#ifndef ALBUM_HPP
#define ALBUM_HPP

#include "Song.hpp"
#include "../utils/Serializer.hpp"

#include <QString>
#include <QtTypes>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMetaEnum>
#include <functional>

class Album {

public:
    enum Category{
        POP,
        ROCK,
        BLUES,
        JAZZ,
        RAP,
        CLASSIC,
        FRENCH,
        ELECTRO,
    };

    enum SupportType{
        LP,
        K7,
        CD,
        MP3,
        M4A,
        FLAC,
    };

private:
    QString title, artistName, compositorName, personalNotes, imageCover;
    quint16 editionYear;
    double euroPrice;
    Category category;
    SupportType supportType;
    std::vector<Song> songs;

public:
    Album();
    ~Album();

    inline void setTitle(QString _title) {
        title = _title;
    }

    inline void setArtistName(QString _artistName){
        artistName = _artistName;
    }

    inline void setCompositorName(QString _compositorName){
        compositorName = _compositorName;
    }

    inline void setEditionYear(quint16 _editionYear){
        editionYear = _editionYear;
    }

    inline void setEuroPrice(double _euroPrice){
        euroPrice = _euroPrice;
    }

    inline void setImageCover(QString _imageCover){
        imageCover = _imageCover;
    }

    inline void setCategory(Category _category){
        category = _category;
    }

    inline void setSupportType(SupportType _supportType){
        supportType = _supportType;
    }
    inline void setPersonalNotes(QString _personalNotes){
        personalNotes = _personalNotes;
    }

    inline QString getTitle() const {
        return title;
    }

    inline QString getArtistName() const {
        return artistName;
    }

    inline QString getCompositorName() const {
        return compositorName;
    }

    inline quint16 getEditionYear() const {
        return editionYear;
    }

    inline double getEuroPrice() const {
        return euroPrice;
    }

    inline QString getImageCover() const {
        return imageCover;
    }

    inline Category getCategory() const {
        return category;
    }

    inline SupportType getSupportType() const {
        return supportType;
    }

    inline QString getPersonalNotes() const {
        return personalNotes;
    }

    inline void addSong(Song song) {
        this->songs.push_back(song);
    }

    bool verifyOutputIntegrity() {
        if (this->title.isNull() || this->title.isEmpty())
            return false;

        if (this->artistName.isNull() || this->artistName.isEmpty())
            return false;

        if (this->compositorName.isNull() || this->compositorName.isEmpty())
            return false;

        return true;
    }

    QJsonObject toJson() {
        if (!verifyOutputIntegrity())
            throw std::invalid_argument("Invalid Album to serialize !");

        QJsonObject obj;

        obj["title"] = this->title;
        obj["artistName"] = this->artistName;
        obj["compositorName"] = this->compositorName;
        obj["editionYear"] = this->editionYear;
        obj["euroPrice"] = this->euroPrice;
        obj["imageCover"] = this->imageCover;
        obj["category"] = Serializer::serializeEnumValue(this->category);
        obj["supportType"] = Serializer::serializeEnumValue(this->supportType);
        obj["personalNotes"] = this->personalNotes;

        QJsonObject songsObj;

        size_t currIndex = 0;
        for (const Song& song : this->songs)
            songsObj[QString::number(currIndex++)] = song.toJson();

        obj["songs"] = songsObj;

        return obj;
    }

    static bool verifyInputIntegrity(const QJsonObject &obj) {
        static std::map<QString, std::function<bool(QJsonValue)>> requiredKeys = {
            {"title", [](QJsonValue value) -> bool {
                 return value.isString();
            }},
            {"artistName", [](QJsonValue value) -> bool {
                 return value.isString();
            }},
            {"compositorName", [](QJsonValue value) -> bool {
                 return value.isString();
            }},
            {"editionYear", [](QJsonValue value) -> bool {
                 return Deserializer::isQuint16(value);
            }},
            {"euroPrice", [](QJsonValue value) -> bool {
                 return value.isDouble();
            }},
            {"imageCover", [](QJsonValue value) -> bool {
                 return value.isString();
            }},
            {"category", [](QJsonValue value) -> bool {
                 return Deserializer::isValidEnumValue<Category>(value);
            }},
            {"supportType", [](QJsonValue value) -> bool {
                 return Deserializer::isValidEnumValue<SupportType>(value);
            }},
            {"personalNotes", [](QJsonValue value) -> bool {
                 return value.isString();
            }}
        };

        for (const auto& pair : requiredKeys)
            if (!obj.contains(pair.first) || !pair.second(obj[pair.first]))
                return false;

        return true;
    }

    static Album fromJson(const QJsonObject& obj) {
        if (!Album::verifyInputIntegrity(obj))
            throw std::invalid_argument("Invalid Album to deserialize !");

        QString title = obj["title"].toString();
        QString artistName = obj["artistName"].toString();
        QString compositorName = obj["compositorName"].toString();
        quint16 editionYear = obj["editionYear"].toInt();
        double euroPrice = obj["euroPrice"].toDouble();
        QString imageCover = obj["imageCover"].toString();
        Category category = Deserializer::parseEnumValue<Category>(obj["category"]);
        SupportType supportType = Deserializer::parseEnumValue<SupportType>(obj["supportType"]);
        QString personalNotes = obj["personalNotes"].toString();

        Album album;

        album.setTitle(title);
        album.setArtistName(artistName);
        album.setCompositorName(compositorName);
        album.setEditionYear(editionYear);
        album.setEuroPrice(euroPrice);
        album.setImageCover(imageCover);
        album.setCategory(category);
        album.setSupportType(supportType);
        album.setPersonalNotes(personalNotes);

        QJsonObject songsObj = obj["songs"].toObject();

        for (auto it = songsObj.constBegin(); it != songsObj.constEnd(); it++) {
            bool ok;
            int index = it.key().toInt(&ok);

            if (ok) {
                Song song = Song::fromJson(it.value().toObject());
                song.setIndex(index);
                album.addSong(song);
            }
        }

        return album;
    }
};


#endif // ALBUM_HPP
