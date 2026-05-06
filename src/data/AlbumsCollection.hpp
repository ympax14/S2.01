#ifndef ALBUMSCOLLECTION_HPP
#define ALBUMSCOLLECTION_HPP

#include "./Album.hpp"

#include <vector>
#include <algorithm>

struct AlbumsCollection {
private:
    std::vector<Album*> albums;

public:
    AlbumsCollection() : albums(std::vector<Album*>()) {};
    ~AlbumsCollection() = default;

    inline const std::vector<Album*>& getAlbums() const {
        return this->albums;
    }

    bool containsAlbum(Album* album) {
        return std::ranges::find(this->albums, album) != this->albums.end();
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

    QJsonObject toJson() {
        QJsonObject obj;

        QJsonArray albumsArr;

        for (auto album : albums)
            albumsArr.append(album->toJson());

        obj["albums"] = albumsArr;

        return obj;
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
};

#endif // ALBUMSCOLLECTION_HPP
