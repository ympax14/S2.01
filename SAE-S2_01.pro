QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ./src/main.cpp \
    ./src/NowPlayingWindow.cpp

HEADERS += \
    ./src/NowPlayingWindow.hpp \
    ./src/utils/Deserializer.hpp \
    ./src/utils/Serializer.hpp \
    ./src/data/Song.hpp

TRANSLATIONS += \
    ./langs/SAE-S2_01_fr_FR.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore