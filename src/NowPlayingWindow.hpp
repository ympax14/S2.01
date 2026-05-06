#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "./data/AlbumsCollection.hpp"

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>

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
    QAction * const openAlbumsCollectionAction,
        * const openAlbumAction;

    QMenu * const saveMenu;
    QAction * const saveAlbumsCollectionAction,
        * const saveAlbumAction;

    QSplitter * const splitter;
    QHBoxLayout * const mainLayout;
public:
    inline const AlbumsCollection& getAlbumsCollection() const {
        return this->albumsCollection;
    }

    void setupWindow();
    void connectActions();

    void loadAlbumsCollection();
    void saveAlbumsCollection();

    void loadAlbum();
    void saveAlbum();
public slots:
    void toast(const QString& title, const QString& description, const ToastPreset& preset, int duration);
    void infoToast(const QString& title, const QString& descriptions, int duration = 4000);
    void errorToast(const QString& title, const QString& descriptions, int duration = 4000);
    void successToast(const QString& title, const QString& description, int duration = 4000);
};
#endif // MAINWINDOW_HPP
