#include "NowPlayingWindow.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

void loadTranslations(QApplication &a) {
    QTranslator translator;

    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SAE-S2_01_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    loadTranslations(app);

    NowPlayingWindow window;
    window.show();

    return QCoreApplication::exec();
}
