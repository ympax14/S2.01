#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <QPixmap>
#include <QEventLoop>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <regex>
#include <map>

class Network {
public:
    static bool isUrl(const std::string string) {
        if (string.empty())
            throw std::invalid_argument("Specified String is empty");

        std::regex pattern(R"(^https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*)$)");
        return std::regex_match(string, pattern);
    }

    static bool isUrl(const QString& string) {
        if (string.isNull() || string.isEmpty())
            throw std::invalid_argument("Specified String is null or empty");

        return Network::isUrl(string.toStdString());
    }

    static QPixmap fetchImage(const QString& imageUrl) {
        static std::map<QString, QPixmap> coverCache;

        if (coverCache.contains(imageUrl))
            return coverCache.at(imageUrl);

        QNetworkAccessManager networkManager;
        QNetworkReply *reply = networkManager.get(QNetworkRequest(QUrl(imageUrl)));

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QPixmap pixmap;
        if (reply->error() == QNetworkReply::NoError) {
            pixmap.loadFromData(reply->readAll());
        } else {
            qDebug() << "Error fetching image:" << reply->errorString();
        }
        reply->deleteLater();

        coverCache[imageUrl] = pixmap;

        return pixmap;
    }
};

#endif // NETWORK_HPP
