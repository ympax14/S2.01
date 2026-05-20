#ifndef DETAILSWIDGET_HPP
#define DETAILSWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "./data/Album.hpp"

class DetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailsWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout * const mainLayout;
    QFormLayout * const informationsLayout;

    QGraphicsScene * const imageScene;
    QGraphicsView * const imageView;

    void loadImage(const QString& path);
    void updateImageView();

    void buildAlbumDetails(Album * album);
    void buildSongDetails(Song * song);
public slots:
    void onAlbumSelection(Album* album);
    void onSongSelection(Song* song);
};

#endif // DETAILSWIDGET_HPP
