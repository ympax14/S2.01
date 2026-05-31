#ifndef DETAILSWIDGET_HPP
#define DETAILSWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextEdit>

#include "./data/Album.hpp"

class DetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailsWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout * const mainLayout;
    QFormLayout * const informationsLayout;
    QTextEdit * const notesEdit;

    QGraphicsScene * const imageScene;
    QGraphicsView * const imageView;

    void loadImage(const QString& path);
    void clearImage();

signals:
    void dataChanged();

public:
    void buildAlbumDetails(Album * album);
    void buildSongDetails(Album * const album, size_t songIndex);
};

#endif // DETAILSWIDGET_HPP
