#ifndef ALBUMLISTWIDGETITEM_H
#define ALBUMLISTWIDGETITEM_H

#include <QListWidgetItem>

class AlbumListWidgetItem : public QListWidgetItem
{
public:
    AlbumListWidgetItem(const QString &text, int realIndex, QListWidget *parent=nullptr);
    int getRealIndex() const {
        return _realIndex;
    }
private:
    int _realIndex;
};

#endif // ALBUMLISTWIDGETITEM_H
