#include "DetailsWidget.hpp"
#include "./utils/Network.hpp"

#include <QLabel>

DetailsWidget::DetailsWidget(QWidget *parent) :
    QWidget{parent},

    mainLayout(new QVBoxLayout(this)),
    informationsLayout(new QFormLayout),

    imageScene(new QGraphicsScene(this)),
    imageView(new QGraphicsView(this))
{
    this->imageView->setScene(this->imageScene);
}

void DetailsWidget::loadImage(const QString& path) {
    this->imageScene->clear();

    QPixmap pixmap;
    if (Network::isUrl(path)) {
        pixmap = Network::fetchImage(path);
    } else {
        pixmap.load(path);
    }

    this->imageScene->addPixmap(pixmap);
    this->imageScene->setSceneRect(pixmap.rect());
    this->imageView->update();
}

void clearLayout(QLayout *layout) {
    while (layout->count() > 0) {
        QLayoutItem *item = layout->takeAt(0);

        // Delete nested layouts if present
        if (QLayout *childLayout = item->layout()) {
            clearLayout(childLayout);
            delete childLayout;
        }

        // Delete the widget
        if (QWidget *widget = item->widget()) {
            delete widget;
        }

        // Delete spacers
        if (QSpacerItem *spacer = item->spacerItem()) {
            delete spacer;
        }

        // Delete the layout item itself
        delete item;
    }
}

void DetailsWidget::buildAlbumDetails(Album * album) {
    this->informationsLayout->addRow(new QLabel("Title:"), new QLabel(album->getTitle()));
    this->informationsLayout->addRow(new QLabel("Artist:"), new QLabel(album->getArtistName()));
    this->informationsLayout->addRow(new QLabel("Compositor:"), new QLabel(album->getCompositorName()));
    this->informationsLayout->addRow(new QLabel("Category:"), new QLabel(QVariant::fromValue(album->getCategory()).value<QString>()));
    this->informationsLayout->addRow(new QLabel("Edition Year:"), new QLabel(QString::number(album->getEditionYear())));
    this->informationsLayout->addRow(new QLabel("Support:"), new QLabel(QVariant::fromValue(album->getSupportType()).value<QString>()));
    this->informationsLayout->addRow(new QLabel("Price:"), new QLabel(QString::number(album->getEuroPrice())));
    this->informationsLayout->addRow(new QLabel("Songs:"), new QLabel(QString::number(album->getSongsCount())));
    this->informationsLayout->addRow(new QLabel("Notes:"), new QLabel(album->getPersonalNotes()));

}

void DetailsWidget::onAlbumSelection(Album* album) {
    clearLayout(this->informationsLayout);
    this->buildAlbumDetails(album);
}

void DetailsWidget::buildSongDetails(Song * song) {

}

void DetailsWidget::onSongSelection(Song* song) {
    clearLayout(this->informationsLayout);
    this->buildSongDetails(song);
}