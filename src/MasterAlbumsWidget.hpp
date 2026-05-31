#ifndef MASTERALBUMSWIDGET_HPP
#define MASTERALBUMSWIDGET_HPP

#include <QObject>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "./data/AlbumsCollection.hpp"
class MasterAlbumsWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel * const labelTitle;
    QLineEdit * const lineEditFilter; // research bar
    QTreeWidget * const treeWidget;

    QPushButton * const buttonNewAlbum,
        * const buttonRemoveAlbum,
        * const buttonSaveAlbum,
        * const buttonAddAlbum,
        * const buttonNewSong,
        * const buttonRemoveSong;

public:
    explicit MasterAlbumsWidget(QWidget *parent = nullptr);

    inline QString getFilter() const {
        return this->lineEditFilter->text();
    }

    void refresh(const AlbumsCollection &collection, const QString &filter = "");

signals:
    void albumSelected(size_t realIndex); // param realIndex c'est l'index de l'album dans albums[]
    void songSelected(size_t albumIndex, size_t songIndex);
    void addAlbumRequested(); // when the user clicks on the button + album, basically adds an album.
    void removeAlbumRequested(size_t realIndex); // for whenever the user clicks on the button remove album
    void saveAlbumRequested(size_t realIndex);
    void newAlbumRequested();
    void newSongRequested(size_t albumIndex);
    void removeSongRequested(size_t albumIndex, size_t songIndex);
    void filterChanged(const QString& filter);
private slots:
    void onSelectionChanged(); // signal for when user clicks on another album
    void onAddAlbum(); // signal for when user adds music
    void onSaveAlbum(); // signal for when user saves album
    void onRemoveAlbum(); // signal for when user removes music
    void onNewAlbum();
    void onNewSong();
    void onRemoveSong();
    void onFilterChanged(const QString& text);

};

#endif // MASTERALBUMSWIDGET_HPP
