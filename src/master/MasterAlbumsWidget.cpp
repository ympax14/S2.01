#include "MasterAlbumsWidget.hpp"

MasterAlbumsWidget::MasterAlbumsWidget(QWidget *parent): QWidget{parent}
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);


    // for the title
    labelTitle = new QLabel(tr("Albums"), this);
    labelTitle->setAlignment(Qt::AlignCenter); // horizontal center
    QFont font = labelTitle->font();
    font.setPointSize(14);
    font.setBold(true);
    labelTitle->setFont(font);


    // for the research bar
    lineEditFilter = new QLineEdit(this);
    lineEditFilter->setPlaceholderText(tr("Search ..."));


    // tree structure : albums & songs
    // each album = item parent
    // each song = item child
    treeWidget = new QTreeWidget(this);
    treeWidget-> setHeaderHidden(true);
    treeWidget->setRootIsDecorated(true); // displays the arrow ->


    // Buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonAdd = new QPushButton(tr("➕ Add album"),this);
    buttonRemove = new QPushButton(tr("➖ Remove album"),this);

    buttonsLayout->addWidget(buttonAdd);
    buttonsLayout->addWidget(buttonRemove);

    // add everything onto the vertical layout
    mainLayout->addWidget(labelTitle);
    mainLayout->addWidget(lineEditFilter);
    mainLayout->addWidget(treeWidget);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    // connect signals & slots
    connect(treeWidget, &QTreeWidget::itemSelectionChanged, this, &MasterAlbumsWidget::onSelectionChanged);
    connect(buttonAdd, &QPushButton::clicked,this, &MasterAlbumsWidget::onAddButtonClicked);
    connect(buttonRemove, &QPushButton::clicked, this, &MasterAlbumsWidget::onRemoveButtonClicked);
    connect(lineEditFilter, &QLineEdit::textChanged, this, &MasterAlbumsWidget::onFilterChanged);

}


void MasterAlbumsWidget::onSelectionChanged(){
    QList<QTreeWidgetItem*> selected = treeWidget->selectedItems();
    if(selected.isEmpty()){
        return;
    }

    QTreeWidgetItem *item = selected.first();
    if(item-> parent() != nullptr){
        return; // its a song then
    }

    int realIndex = item->data(0, Qt::UserRole).toInt();
    emit albumSelected(realIndex);
}


void MasterAlbumsWidget::onAddButtonClicked(){
    emit addAlbumRequested();
}

void MasterAlbumsWidget::onRemoveButtonClicked(){
    QList<QTreeWidgetItem*> selected = treeWidget->selectedItems();
    if (selected.isEmpty()){
        return;
    }

    QTreeWidgetItem *item = selected.first();
    if(item->parent() !=  nullptr){
        return;
    }

    int realIndex = item->data(0, Qt::UserRole).toInt();
    emit removeAlbumRequested(realIndex);
}

void MasterAlbumsWidget::onFilterChanged(const  QString &text)
{
    emit filterChanged(text);
}



void MasterAlbumsWidget::refresh(const AlbumsCollection &collection, const QString &filter){
    treeWidget->clear();

    int realIndex= 0 ; // index of the album , located in the collections' vector
    for (Album *album : collection.getAlbums()){

        // applying filter
        // if filter is used, checking if the title or the artist exists
        if(!filter.isEmpty()){
            QRegularExpression regexpression(filter, QRegularExpression::CaseInsensitiveOption);
            if(!album ->getTitle().contains(regexpression) && !album->getArtistName().contains(regexpression)){
                realIndex++;
                continue;
            }
        }
        // album (item parent)
        QTreeWidgetItem *albumItem = new QTreeWidgetItem(treeWidget);
        albumItem->setText(0,QString("%1 - %2").arg(album->getArtistName(),  album->getTitle()));
        albumItem->setData(0,Qt::UserRole, realIndex);


        // song (item child)
        for (const Song &song : album->getSongs()) {
            QTreeWidgetItem *songItem = new QTreeWidgetItem(albumItem);


            // time's format minutes:seconds
            int minutes = song.getDuration() / 60;
            int seconds = song.getDuration() % 60;
            QString duration = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

            songItem->setText(0, QString("  %1. %2 (%3)").arg(song.getIndex()).arg(song.getTitle()).arg(duration));

            // songs cannot b selected, we can only select albums
            songItem->setFlags(songItem->flags() & ~Qt::ItemIsSelectable);
        }

        realIndex++;
    }
}

