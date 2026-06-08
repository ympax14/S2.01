#include "MasterAlbumsWidget.hpp"

MasterAlbumsWidget::MasterAlbumsWidget(QWidget *parent) :
    QWidget{parent},

    labelTitle(new QLabel(tr("Albums"), this)),
    lineEditFilter(new QLineEdit(this)),
    treeWidget(new QTreeWidget(this)),

    buttonNewAlbum(new QPushButton(tr("➕ Album"), this)),
    buttonRemoveAlbum(new QPushButton(tr("➖ Album"), this)),
    buttonSaveAlbum(new QPushButton(tr("💾 Album"), this)),
    buttonAddAlbum(new QPushButton(tr("🗁 Album"), this)),
    buttonNewSong(new QPushButton(tr("➕ Song"), this)),
    buttonRemoveSong(new QPushButton(tr("➖ Song"), this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // for the title
    labelTitle->setAlignment(Qt::AlignCenter); // horizontal center
    labelTitle->setStyleSheet(
        "background-color: #2c2c2e;"
        "color: #fc3c44;"
        "font-size: 18px;"
        "font-weight: bold;"
        "padding: 12px;"
        "border-radius: 4px;"
        "letter-spacing: 3px;"
        );
    QFont font = labelTitle->font();
    font.setPointSize(14);
    font.setBold(true);
    labelTitle->setFont(font);

    // for the research bar
    lineEditFilter->setPlaceholderText(tr("Search ..."));

    // tree structure : albums & songs
    // each album = item parent
    // each song = item child
    treeWidget-> setHeaderHidden(true);
    treeWidget->setRootIsDecorated(true); // displays the arrow ->

    // Buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    buttonsLayout->addWidget(buttonNewAlbum);
    buttonsLayout->addWidget(buttonRemoveAlbum);
    buttonsLayout->addWidget(buttonSaveAlbum);
    buttonsLayout->addWidget(buttonAddAlbum);
    buttonsLayout->addWidget(buttonNewSong);
    buttonsLayout->addWidget(buttonRemoveSong);

    // add everything onto the vertical layout
    mainLayout->addWidget(labelTitle);
    mainLayout->addWidget(lineEditFilter);
    mainLayout->addWidget(treeWidget);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    // connect signals & slots
    connect(treeWidget, &QTreeWidget::itemSelectionChanged, this, &MasterAlbumsWidget::onSelectionChanged);
    connect(buttonAddAlbum, &QPushButton::clicked,this, &MasterAlbumsWidget::onAddAlbum);
    connect(buttonRemoveAlbum, &QPushButton::clicked, this, &MasterAlbumsWidget::onRemoveAlbum);
    connect(buttonSaveAlbum, &QPushButton::clicked, this, &MasterAlbumsWidget::onSaveAlbum);
    connect(buttonNewAlbum, &QPushButton::clicked, this, &MasterAlbumsWidget::onNewAlbum);
    connect(buttonNewSong, &QPushButton::clicked, this, &MasterAlbumsWidget::onNewSong);
    connect(buttonRemoveSong, &QPushButton::clicked, this, &MasterAlbumsWidget::onRemoveSong);
    connect(lineEditFilter, &QLineEdit::textChanged, this, &MasterAlbumsWidget::onFilterChanged);
}

void MasterAlbumsWidget::onSelectionChanged(){
    QList<QTreeWidgetItem*> selected = treeWidget->selectedItems();
    if (selected.isEmpty()) return;

    QTreeWidgetItem *item = selected.first();

    if (item->parent() != nullptr) {
        size_t songIndex = item->data(0, Qt::UserRole).toULongLong();
        size_t albumIndex = item->parent()->data(0, Qt::UserRole).toULongLong();

        qDebug() << "Song id: " << songIndex << ", album id: " << albumIndex;

        emit songSelected(albumIndex, songIndex);
    } else {
        size_t realIndex = item->data(0, Qt::UserRole).toULongLong();
        qDebug() << "Album id: " << realIndex;
        emit albumSelected(realIndex);
    }
}

void MasterAlbumsWidget::onNewAlbum() {
    emit newAlbumRequested();
}

void MasterAlbumsWidget::onNewSong() {
    QList<QTreeWidgetItem*> selected = this->treeWidget->selectedItems();
    if (selected.isEmpty()) return;

    QTreeWidgetItem* selectedItem = selected.first();

    // If a song is selected, use its parent album
    QTreeWidgetItem* albumItem = selectedItem->parent() ? selectedItem->parent() : selectedItem;

    size_t albumIndex = albumItem->data(0, Qt::UserRole).toULongLong();
    emit newSongRequested(albumIndex);
}

void MasterAlbumsWidget::onRemoveSong() {
    QList<QTreeWidgetItem*> selected = this->treeWidget->selectedItems();
    if (selected.isEmpty()) return;

    QTreeWidgetItem* selectedItem = selected.first();

    // Check if the selected item is a childitem
    if (!selectedItem->parent()) return;

    size_t songIndex = selectedItem->data(0, Qt::UserRole).toULongLong();
    size_t albumIndex = selectedItem->parent()->data(0, Qt::UserRole).toULongLong();
    emit removeSongRequested(albumIndex, songIndex);
}

void MasterAlbumsWidget::onAddAlbum(){
    emit addAlbumRequested();
}

void MasterAlbumsWidget::onSaveAlbum() {
    QList<QTreeWidgetItem*> selected = treeWidget->selectedItems();
    if (selected.isEmpty()){
        return;
    }

    QTreeWidgetItem *item = selected.first();
    if(item->parent() !=  nullptr){
        return;
    }

    size_t realIndex = item->data(0, Qt::UserRole).toULongLong();
    emit saveAlbumRequested(realIndex);
}

void MasterAlbumsWidget::onRemoveAlbum(){
    QList<QTreeWidgetItem*> selected = treeWidget->selectedItems();
    if (selected.isEmpty()){
        return;
    }

    QTreeWidgetItem *item = selected.first();
    if(item->parent() !=  nullptr){
        return;
    }

    size_t realIndex = item->data(0, Qt::UserRole).toULongLong();
    emit removeAlbumRequested(realIndex);
}

void MasterAlbumsWidget::onFilterChanged(const QString &text)
{
    emit filterChanged(text);
}

void MasterAlbumsWidget::refresh(const AlbumsCollection &collection, const QString &filter){
    // Save current selection before clearing
    size_t savedAlbumIndex = SIZE_MAX;
    size_t savedSongIndex = SIZE_MAX;
    {
        QList<QTreeWidgetItem*> selected = treeWidget->selectedItems();
        if (!selected.isEmpty()) {
            QTreeWidgetItem* item = selected.first();
            if (item->parent()) {
                savedAlbumIndex = item->parent()->data(0, Qt::UserRole).toULongLong();
                savedSongIndex = item->data(0, Qt::UserRole).toULongLong();
            } else {
                savedAlbumIndex = item->data(0, Qt::UserRole).toULongLong();
            }
        }
    }

    this->treeWidget->clear();

    QRegularExpression regexpression(filter, QRegularExpression::CaseInsensitiveOption);

    int albumIndex = 0;
    for (Album *album : collection.getAlbums()){

        QTreeWidgetItem *albumItem = new QTreeWidgetItem;
        albumItem->setText(0, QString("%1 - %2").arg(album->getArtistName(), album->getTitle()));
        albumItem->setData(0, Qt::UserRole, albumIndex);

        bool albumMatch = filter.isEmpty() ? true : (album->getTitle().contains(regexpression) || album->getArtistName().contains(regexpression));
        bool albumHasMatchingSongs = false;

        int songIndex = 0;
        for (const Song &song : album->getSongs()) {
            bool songMatch = filter.isEmpty() ? true : song.getTitle().contains(regexpression);
            if (albumMatch || songMatch) {
                if (songMatch && !filter.isEmpty())
                    albumHasMatchingSongs = true;

                QTreeWidgetItem *songItem = new QTreeWidgetItem(albumItem);
                QString duration = song.getFormattedDuration();
                songItem->setText(0, QString("  %1. %2 (%3)").arg(song.getIndex()).arg(song.getTitle()).arg(duration));
                songItem->setData(0, Qt::UserRole, songIndex);
            }
            songIndex++;
        }

        if (albumMatch || albumHasMatchingSongs) {
            this->treeWidget->addTopLevelItem(albumItem);
            if (!filter.isEmpty() && albumHasMatchingSongs)
                albumItem->setExpanded(true);
        } else {
            delete albumItem;
        }

        albumIndex++;
    }

    // Restore previous selection without firing itemSelectionChanged
    if (savedAlbumIndex != SIZE_MAX) {
        treeWidget->blockSignals(true);
        for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
            QTreeWidgetItem* albumItem = treeWidget->topLevelItem(i);
            if (albumItem->data(0, Qt::UserRole).toULongLong() == savedAlbumIndex) {
                if (savedSongIndex == SIZE_MAX) {
                    treeWidget->setCurrentItem(albumItem);
                } else {
                    for (int j = 0; j < albumItem->childCount(); j++) {
                        QTreeWidgetItem* songItem = albumItem->child(j);
                        if (songItem->data(0, Qt::UserRole).toULongLong() == savedSongIndex) {
                            treeWidget->setCurrentItem(songItem);
                            break;
                        }
                    }
                }
                break;
            }
        }
        treeWidget->blockSignals(false);
    }
}

