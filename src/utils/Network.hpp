#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <QPixmap>
#include <QFuture>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QPromise>
#include <regex>

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
        QNetworkAccessManager networkManager;
        QNetworkReply *reply = networkManager.get(QNetworkRequest(QUrl(imageUrl))); // GET request
        QPromise<QPixmap> * prom = new QPromise<QPixmap>;

        QObject::connect(reply, &QNetworkReply::finished, [reply, prom]() {
            prom->start();
            if (reply->error() == QNetworkReply::NoError) {
                // Read data and load into QPixmap
                QPixmap pixmap;
                pixmap.loadFromData(reply->readAll());
                prom->addResult(pixmap);
            } else {
                // Handle error
                qDebug() << "Error:" << reply->errorString();
                prom->addResult(QPixmap());
            }
            prom->finish();
            // Always delete the reply to avoid memory leaks
            reply->deleteLater();
        });

        // Wait for the image to fetch through a Promise
        QFuture<QPixmap> result = prom->future();
        result.waitForFinished();

        return result.result();
    }
};

#endif // NETWORK_HPP
