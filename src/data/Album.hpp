#ifndef ALBUM_H
#define ALBUM_H

#include <QString>
#include <QtTypes>




class Album {

public:
    enum Category{
        POP,
        ROCK,
        BLUES,
        JAZZ,
        RAP,
        CLASSIC,
        FRENCH,
        ELECTRO,
    };
    enum SupportType{
        LP,
        K7,
        CD,
        MP3,
        M4A,
        FLAC,
    };

private:
    QString title;
    QString artistName;
    QString compositorName;
    quint8 editionYear;
    double euroPrice;
    QString cover;
    Category category;
    SupportType supportType;
    QString personalNotes;

public:
    //setters et getters
    Album();
    ~Album();
    inline void setTitle(QString _title) {
        title = _title;
    }

    inline void setArtistName(QString _artistName){
        artistName = _artistName;
    }

    inline void setCompositorName(QString _compositorName){
        compositorName = _compositorName;
    }

    inline void setEditionYear(quint8 _editionYear){
        editionYear = _editionYear;
    }

    inline void setEuroPrice(double _euroPrice){
        euroPrice = _euroPrice;
    }

    inline void setCover(QString _cover){
        cover = _cover;
    }

    inline void setCategory(Category _category){
        category = _category;
    }

    inline void setSupportType(SupportType _supportType){
        supportType = _supportType;
    }
    inline void setPersonalNotes(QString _personalNotes){
        personalNotes = _personalNotes;
    }

    inline QString getTitle(){
        return title;
    }

    inline QString getArtistName(){
        return artistName;
    }

    inline QString getCompositorName(){
        return compositorName;
    }

    inline quint8  getEditionYear(){
        return editionYear;
    }

    inline double getEuroPrice(){
        return euroPrice;
    }

    inline QString getCover(){
        return cover;
    }

    inline Category getCategory(){
        return category;
    }

    SupportType getSupportType(){
        return supportType;
    }

    inline QString getPersonalNotes(){
        return personalNotes;
    }

};


#endif // ALBUM_H
