#ifndef MASTERALBUMSWIDGET_HPP
#define MASTERALBUMSWIDGET_HPP

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>

class AlbumListWidgetItem : public QListWidgetItem
{
public:
    AlbumListWidgetItem(const QString &text, int realIndex, QListWidget *parent=nullptr) : QListWidgetItem(text, parent), _realIndex(realIndex){}
    int getRealIndex() const {
        return _realIndex;
    }
private:
    int _realIndex;
};

#include "src/data/AlbumsCollection.hpp"
class MasterAlbumsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MasterAlbumsWidget(QWidget *parent = nullptr);
    void refresh(const AlbumsCollection &collection, const QString &filter = " ");

signals:
    void albumSelected(int realIndex); // param realIndex c'est l'index de l'album dans albums[]
    void addAlbumRequested(); // when the user clicks on the button + album, basically adds an album.
    void removeAlbumRequested(int realIndex); // for whenever the user clicks on the button remove album.
private slots:
    void onSelectionChanged(); // signal for when user clicks on another album
    void onAddButtonClicked(); // signal for when user adds music
    void onRemoveButtonClicked(); // signal for when user removes music

private:
    QLabel *labelTitle;
    QLineEdit *lineEditFilter; // research bar
    QListWidget *listWidget;
    QPushButton *buttonAdd;
    QPushButton *buttonRemove;

};

#endif // MASTERALBUMSWIDGET_HPP
