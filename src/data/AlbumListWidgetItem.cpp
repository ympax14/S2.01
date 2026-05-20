#include "AlbumListWidgetItem.hpp"

AlbumListWidgetItem::AlbumListWidgetItem(const QString &text, int realIndex, QListWidget *parent): QListWidgetItem(text, parent),_realIndex(realIndex){}
