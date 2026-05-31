#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "./data/AlbumsCollection.hpp"

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>

#include "MasterAlbumsWidget.hpp"
#include "DetailsWidget.hpp"

#include <qt-toast/Toast.h>

class NowPlayingWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int WINDOW_WIDTH, WINDOW_HEIGHT;

    explicit NowPlayingWindow(QWidget *parent = nullptr);
    ~NowPlayingWindow() = default;

private:
    AlbumsCollection albumsCollection;

    QMenu * const openMenu;
    QAction * const openAlbumsCollectionAction;

    QMenu * const saveMenu;
    QAction * const saveAlbumsCollectionAction;

    MasterAlbumsWidget * const masterAlbumsWidget;
    DetailsWidget * const detailsWidget;

    QSplitter * const splitter;
    QHBoxLayout * const mainLayout;
public:
    inline const AlbumsCollection& getAlbumsCollection() const {
        return this->albumsCollection;
    }

    void setupWindow();
    void connectActions();
public slots:
    void loadAlbumsCollection();
    void saveAlbumsCollection();

    void loadAlbum();
    void removeAlbum(size_t index);
    void saveAlbum(size_t index);
    void newAlbum();

    void newSong(size_t albumIndex);
    void removeSong(size_t albumIndex, size_t songIndex);

    void albumSelected(size_t index);
    void songSelected(size_t albumIndex, size_t songIndex);

    void filterChanged(const QString& text) const;

    void toast(const QString& title, const QString& description, const ToastPreset& preset, int duration);
    void infoToast(const QString& title, const QString& descriptions, int duration = 4000);
    void errorToast(const QString& title, const QString& descriptions, int duration = 4000);
    void successToast(const QString& title, const QString& description, int duration = 4000);
};
#endif // MAINWINDOW_HPP
