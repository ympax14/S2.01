QT += widgets network

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ./src/main.cpp \
    ./src/NowPlayingWindow.cpp \
    ./includes/qt-toast/Toast.cpp \
    ./src/DetailsWidget.cpp \
    ./src/master/MasterAlbumsWidget.cpp \
    ./src/master/AlbumListWidgetItem.cpp \

HEADERS += \
    ./src/NowPlayingWindow.hpp \
    ./src/utils/Deserializer.hpp \
    ./src/utils/Serializer.hpp \
    ./src/data/Song.hpp \
    ./src/data/Album.hpp \
    ./src/data/AlbumsCollection.hpp \
    ./includes/qt-toast/Toast.h \
    ./includes/qt-toast/Enums.h \
    ./src/master/MasterAlbumsWidget.hpp \
    ./src/master/AlbumListWidgetItem.hpp \
    ./src/DetailsWidget.hpp \
    ./src/utils/Network.hpp

TRANSLATIONS += \
    ./langs/SAE-S2_01_fr_FR.ts

CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += ./includes

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    tests/Makefile

TOAST_ICONS_DIR = $$PWD/includes/qt-toast/icons
TOAST_CSS_DIR = $$PWD/includes/qt-toast/css

# Copie selon l'OS
win32 {
    # Windows : on utilise xcopy
    # On remplace les / par des \ pour Windows
    source_icons = $$replace(TOAST_ICONS_DIR, /, \\)
    source_css = $$replace(TOAST_CSS_DIR, /, \\)
    dest_icons = $$replace(OUT_PWD/icons, /, \\)
    dest_css = $$replace(OUT_PWD/css, /, \\)

    copy_icons.commands = xcopy /s /e /y /i $$source_icons $$dest_icons
    copy_css.commands = xcopy /s /e /y /i $$source_css $$dest_css
} else {
    # Linux / macOS : on utilise cp
    copy_icons.commands = cp -r $$TOAST_ICONS_DIR $$OUT_PWD/
    copy_css.commands = cp -r $$TOAST_CSS_DIR $$OUT_PWD/
}

# Ajouter les commandes aux étapes de build de QMake
QMAKE_EXTRA_TARGETS += copy_icons copy_css
POST_TARGETDEPS += copy_icons copy_css