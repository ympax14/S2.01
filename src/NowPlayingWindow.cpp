#include "NowPlayingWindow.hpp"

#include <QMenuBar>
#include <QFileDialog>

const int NowPlayingWindow::WINDOW_HEIGHT = 720;
const int NowPlayingWindow::WINDOW_WIDTH = 1280;

NowPlayingWindow::NowPlayingWindow(QWidget *parent) :
    QMainWindow(parent),

    openMenu(this->menuBar()->addMenu("&Open")),
    openAlbumsCollectionAction(this->openMenu->addAction(tr("&Albums Collection"))),
    openAlbumAction(this->openMenu->addAction(tr("&Album"))),

    saveMenu(this->menuBar()->addMenu("&Open")),
    saveAlbumsCollectionAction(this->saveMenu->addAction(tr("&Albums Collection"))),
    saveAlbumAction(this->saveMenu->addAction(tr("&Album"))),

    splitter(new QSplitter(Qt::Horizontal, this)),
    mainLayout(new QHBoxLayout(this->splitter))
{
    this->setupWindow();
    this->connectActions();
}

void NowPlayingWindow::setupWindow() {
    this->resize(NowPlayingWindow::WINDOW_WIDTH, NowPlayingWindow::WINDOW_HEIGHT);
    this->setCentralWidget(this->splitter);

    this->splitter->addWidget(new QWidget()); // Côté gauche -> Albums et Sons (Master)
    this->splitter->addWidget(new QWidget()); // Côté droit -> Informations (Details)
}

void NowPlayingWindow::connectActions() {
    QObject::connect(this->openAlbumsCollectionAction, &QAction::triggered, this, &NowPlayingWindow::loadAlbumsCollection);
    QObject::connect(this->saveAlbumsCollectionAction, &QAction::triggered, this, &NowPlayingWindow::saveAlbumsCollection);
    QObject::connect(this->openAlbumAction, &QAction::triggered, this, &NowPlayingWindow::loadAlbum);
    QObject::connect(this->saveAlbumAction, &QAction::triggered, this, &NowPlayingWindow::saveAlbum);
}

void NowPlayingWindow::loadAlbumsCollection() {

}

void NowPlayingWindow::saveAlbumsCollection() {

}

void NowPlayingWindow::saveAlbum() {

}

void NowPlayingWindow::loadAlbum() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open album"), QDir::homePath(), tr("Album (*.json)"));

    if (!filePath.isEmpty()) {
        QJsonObject obj = Deserializer::loadJson(filePath);

        try {
            Album *album = Album::fromJson(obj);

            try {
                this->albumsCollection.addAlbum(album);
            } catch (std::invalid_argument& exception) {
                this->errorToast(tr("Error while loading Album !"), tr("This album is already loaded !"));
                qDebug() << "AlbumsCollection already contains this Album";
            }
        } catch (std::invalid_argument& exception) {
            this->errorToast(tr("Error while loading Album !"), tr("Invalid album file !"));
            qDebug() << "Invalid album file " + filePath;
        }
    }
}

void NowPlayingWindow::toast(const QString& title, const QString& description, const ToastPreset& preset, int duration) {
    Toast* toast = new Toast(this);
    toast->setDuration(duration);
    toast->setTitle(title);
    toast->setText(description);
    toast->applyPreset(preset);
    toast->setPosition(ToastPosition::BOTTOM_RIGHT);
    toast->show();
}

void NowPlayingWindow::infoToast(const QString& title, const QString& description, int duration) {
    this->toast(title, description, ToastPreset::INFORMATION, duration);
}

void NowPlayingWindow::errorToast(const QString& title, const QString& description, int duration) {
    this->toast(title, description, ToastPreset::ERROR, duration);
}

void NowPlayingWindow::successToast(const QString& title, const QString& description, int duration) {
    this->toast(title, description, ToastPreset::ERROR, duration);
}