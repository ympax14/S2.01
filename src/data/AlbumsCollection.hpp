#ifndef ALBUMSCOLLECTION_HPP
#define ALBUMSCOLLECTION_HPP

#include "./Album.hpp"

#include <vector>
#include <algorithm>
#include <QDomDocument>

struct AlbumsCollection {
private:
    std::vector<Album*> albums;

public:
    AlbumsCollection() : albums(std::vector<Album*>()) {};
    ~AlbumsCollection() = default;

    inline const std::vector<Album*>& getAlbums() const {
        return this->albums;
    }

    inline size_t getSize() const {
        return this->albums.size();
    }

    bool containsAlbum(Album* album) {
        return album->getTitle() == "New Album" ? false : std::ranges::find(this->albums, album) != this->albums.end();
    }

    Album* getAlbum(size_t index) {
        if (index >= this->getSize())
            throw std::out_of_range("AlbumsCollection doesn't contains this Album");
        else
            return this->albums[index];
    }

    void addAlbum(Album* album) {
        if (this->containsAlbum(album))
            throw std::invalid_argument("AlbumsCollection already contains this Album");

        this->albums.push_back(album);
    }

    void removeAlbum(Album* album) {
        auto it = std::ranges::find(this->albums, album);

        if (it != this->albums.end())
            this->albums.erase(it);
        else
            throw std::invalid_argument("AlbumsCollection doesn't contains this Album");
    }

    void removeAlbum(size_t index) {
        if (index >= this->getSize())
            throw std::out_of_range("AlbumsCollection doesn't contains this Album");
        else
            this->albums.erase(this->albums.begin() + index);
    }

    QJsonObject toJson() {
        QJsonObject obj;

        QJsonArray albumsArr;

        for (auto album : albums)
            albumsArr.append(album->toJson());

        obj["albums"] = albumsArr;

        return obj;
    }

    QDomDocument toXml() {
        QDomDocument doc;

        QDomElement root = doc.createElement("AlbumsCollection");
        doc.appendChild(root);

        //  permet l'ajout de chaque album dans le XML
        for (auto album : albums)
            root.appendChild(album->toXml(doc));

        return doc;

    }

    static AlbumsCollection fromJson(const QJsonObject& obj) {
        if (!obj.contains("albums") || !obj["albums"].isArray())
            throw std::invalid_argument("Invalid AlbumsCollection to deserialize");

        AlbumsCollection collection;

        QJsonArray albumsArr = obj["albums"].toArray();

        for (auto it = albumsArr.constBegin(); it != albumsArr.constEnd(); it++)
            collection.addAlbum(Album::fromJson(it->toObject()));

        return collection;
    }

    static AlbumsCollection fromXml(const QDomDocument& doc) {
        // Récupère la racine <AlbumsCollection>
        QDomElement root = doc.documentElement();

        if (root.isNull())
            throw std::invalid_argument("Invalid AlbumsCollection to deserialize");

        AlbumsCollection collection;

        QDomNodeList albumNodes = root.elementsByTagName("album");
        for (int i = 0; i < albumNodes.count(); i++)
            collection.addAlbum(Album::fromXml(albumNodes.at(i).toElement()));

        return collection;
    }
};

#endif // ALBUMSCOLLECTION_HPP
