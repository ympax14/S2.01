#include "NowPlayingWindow.hpp"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

const int NowPlayingWindow::WINDOW_HEIGHT = 720;
const int NowPlayingWindow::WINDOW_WIDTH = 1280;

NowPlayingWindow::NowPlayingWindow(QWidget *parent) :
    QMainWindow(parent),

    openMenu(this->menuBar()->addMenu("&Open")),
    openAlbumsCollectionAction(this->openMenu->addAction(tr("&Albums Collection"))),

    saveMenu(this->menuBar()->addMenu("&Save")),
    saveAlbumsCollectionAction(this->saveMenu->addAction(tr("&Albums Collection"))),
     saveAlbumsCollectionAsAction(this->saveMenu->addAction(tr("&Save As..."))),

    masterAlbumsWidget(new MasterAlbumsWidget),
    detailsWidget(new DetailsWidget),

    splitter(new QSplitter(Qt::Horizontal, this)),
    mainLayout(new QHBoxLayout(this->splitter))
{
    this->setupWindow();
    this->connectActions();
}

NowPlayingWindow::~NowPlayingWindow() {}

void NowPlayingWindow::setupWindow() {
    this->resize(NowPlayingWindow::WINDOW_WIDTH, NowPlayingWindow::WINDOW_HEIGHT);
    this->setCentralWidget(this->splitter);

    this->detailsWidget->hide();

    this->splitter->addWidget(this->masterAlbumsWidget); // Côté gauche -> Albums et Sons (Master)
    this->splitter->addWidget(this->detailsWidget); // Côté droit -> Informations (Details)
}

void NowPlayingWindow::connectActions() {
    QObject::connect(this->openAlbumsCollectionAction, &QAction::triggered, this, &NowPlayingWindow::loadAlbumsCollection);
    QObject::connect(this->saveAlbumsCollectionAction, &QAction::triggered, this, &NowPlayingWindow::saveAlbumsCollection);
    QObject::connect(this->saveAlbumsCollectionAsAction, &QAction::triggered, this, &NowPlayingWindow::saveAlbumsCollectionAs);

    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::addAlbumRequested, this, &NowPlayingWindow::loadAlbum);
    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::removeAlbumRequested, this, &NowPlayingWindow::removeAlbum);
    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::saveAlbumRequested, this, &NowPlayingWindow::saveAlbum);
    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::newAlbumRequested, this, &NowPlayingWindow::newAlbum);

    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::newSongRequested, this, &NowPlayingWindow::newSong);
    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::removeSongRequested, this, &NowPlayingWindow::removeSong);

    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::albumSelected, this, &NowPlayingWindow::albumSelected);
    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::songSelected, this, &NowPlayingWindow::songSelected);

    QObject::connect(this->masterAlbumsWidget, &MasterAlbumsWidget::filterChanged, this, &NowPlayingWindow::filterChanged);

    QObject::connect(this->detailsWidget, &DetailsWidget::dataChanged, this, [this]() {
        this->masterAlbumsWidget->refresh(this->albumsCollection, this->masterAlbumsWidget->getFilter());
        this->unsavedChanges = true;
    });
}

void NowPlayingWindow::albumSelected(size_t index) {
    this->detailsWidget->show();

    Album *album = this->albumsCollection.getAlbum(index);
    this->detailsWidget->buildAlbumDetails(album);
}

void NowPlayingWindow::songSelected(size_t albumIndex, size_t songIndex) {
    this->detailsWidget->show();

    Album *album = this->albumsCollection.getAlbum(albumIndex);
    this->detailsWidget->buildSongDetails(album, songIndex);
}

void NowPlayingWindow::filterChanged(const QString& text) const {
    this->masterAlbumsWidget->refresh(this->getAlbumsCollection(), text);
}

void NowPlayingWindow::loadAlbumsCollection() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open AlbumsCollection"), QDir::homePath(), "AlbumsCollection (*.json)");

    if (!filePath.isEmpty()) {
        QJsonObject obj = Deserializer::loadJson(filePath);

        try {
            AlbumsCollection albumsColl = AlbumsCollection::fromJson(obj);

            this->albumsCollection = albumsColl;
            this->currentFilePath = filePath;
            this->unsavedChanges = false;
            this->masterAlbumsWidget->refresh(this->albumsCollection);
            this->successToast(tr("Collection Loaded"), tr("Collection loaded successfully"));
        } catch (std::invalid_argument& exception) {
            this->errorToast(tr("Error while loading the album's collection !"), tr("Invalid collection file !"));
            qDebug() << "Invalid collection file " + filePath;
        }
    }
}

void NowPlayingWindow::newSong(size_t albumIndex) {
    Album *album = this->albumsCollection.getAlbum(albumIndex);

    Song song;
    if (album->getSongsCount() > 0)
        song.setIndex(album->getSong(album->getSongsCount() - 1).getIndex() + 1);
    else
        song.setIndex(1);

    album->addSong(song);
    this->masterAlbumsWidget->refresh(this->albumsCollection, this->masterAlbumsWidget->getFilter());
    this-> unsavedChanges = true;
    this->infoToast(tr("New Song !"), tr("a new song has been added"));
}

void NowPlayingWindow::removeSong(size_t albumIndex, size_t songIndex) {
    Album *album = this->albumsCollection.getAlbum(albumIndex);
    album->removeSong(songIndex);
    this->masterAlbumsWidget->refresh(this->albumsCollection, this->masterAlbumsWidget->getFilter());
    this->unsavedChanges = true;
    this->infoToast(tr("Song Removed"), tr(" a song has been removed."));

}


void NowPlayingWindow::newAlbum() {
    this->albumsCollection.addAlbum(new Album);
    this->masterAlbumsWidget->refresh(this->albumsCollection, this->masterAlbumsWidget->getFilter());
    this->unsavedChanges = true;
    this->successToast(tr("New Album"), tr("a new album has been added"));
}

void NowPlayingWindow::saveAlbum(size_t realIndex) {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Album"), QDir::homePath(), "Album (*.json)");

    if (!filePath.isEmpty()) {
        if (!filePath.contains(".json"))
            filePath.append(".json");

        Album* album = this->albumsCollection.getAlbum(realIndex);
        QJsonObject jsonObj = album->toJson();
        Serializer::saveData(filePath, jsonObj);
    }
}

void NowPlayingWindow::removeAlbum(size_t index) {
    if (index >= this->albumsCollection.getSize())
        throw std::out_of_range("L'index spécifié n'existe pas");

    this->albumsCollection.removeAlbum(index);
    this->masterAlbumsWidget->refresh(this->albumsCollection, this->masterAlbumsWidget->getFilter());
    this->infoToast(tr("Removed"), tr("Album removed successfully !"));
    this->unsavedChanges = true;
}

void NowPlayingWindow::loadAlbum() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open album"), QDir::homePath(), "Album (*.json)");

    if (!filePath.isEmpty()) {
        QJsonObject obj = Deserializer::loadJson(filePath);

        try {
            Album *album = Album::fromJson(obj);

            try {
                this->albumsCollection.addAlbum(album);
                this->masterAlbumsWidget->refresh(this->albumsCollection);
            } catch (std::invalid_argument& exception) {
                this->errorToast(tr("Error while loading Album !"), tr("This album is already loaded !"));
                qDebug() << "the Album's Collection already contains this Album";
            }
        } catch (std::invalid_argument& exception) {
            this->errorToast(tr("Error while loading Album !"), tr("Invalid album file !"));
            qDebug() << "Invalid album file " + filePath;
        }
    }
}

void NowPlayingWindow::saveAlbumsCollection(){
    QString filePath = this->currentFilePath;
    //CASE 1 : no file loaded yet, ask user where to save
    if (filePath.isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, tr("Save AlbumsCollection"), QDir::homePath(), "AlbumsCollection (*.json)");
        if (filePath.isEmpty()) return;
        if (!filePath.endsWith(".json"))
            filePath.append(".json");
        this->currentFilePath = filePath; // memorizes the path for future saves
    }

    try {
        QJsonObject jsonObj = this->albumsCollection.toJson();
        Serializer::saveData(filePath, jsonObj);
        unsavedChanges = false;
        this->successToast(tr("Saved"), tr("Collection saved successfully :>"));
        // 1 or more albums are invalid (example : empty title or artist name)
    } catch (std::invalid_argument&) {
        this->errorToast(tr("Save failed !!"), tr("One or more albums are invalid (probably missing title or artist?) "));
    }
}

void NowPlayingWindow::saveAlbumsCollectionAs(){
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Collection As"), QDir::homePath(), "Collection (*.json)");
    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".json")) {
        filePath.append(".json");
    }

    QJsonObject jsonObj = this->albumsCollection.toJson();
    Serializer::saveData(filePath, jsonObj);
    this->currentFilePath = filePath;
    unsavedChanges = false;
    this->successToast(tr("Saved As"), tr("Collection saved to new file successfully :>"));
}

void NowPlayingWindow::closeEvent(QCloseEvent *event){
    if (unsavedChanges) {
        int reply = QMessageBox::warning(
            this,
            tr("Unsaved changes"),
            tr("You have unsaved changes. Do you perhaps want to save before quitting ?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            saveAlbumsCollection();
           // check if saving was successful
            if (!unsavedChanges) {
                event->accept();
            } else {
                event->ignore();
            }
        } else if (reply == QMessageBox::Discard) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
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
    this->toast(title, description, ToastPreset::SUCCESS, duration);
}
