#include "NowPlayingWindow.hpp"

const int NowPlayingWindow::WINDOW_HEIGHT = 720;
const int NowPlayingWindow::WINDOW_WIDTH = 1280;

NowPlayingWindow::NowPlayingWindow(QWidget *parent) :
    QMainWindow(parent),

    mainLayout(new QHBoxLayout(this)),
    splitter(new QSplitter(Qt::Horizontal, this))
{
    this->resize(NowPlayingWindow::WINDOW_WIDTH, NowPlayingWindow::WINDOW_HEIGHT);
    this->setCentralWidget(this->splitter);

    this->splitter->addWidget(new QWidget()); // Côté gauche -> Albums et Sons (Master)
    this->splitter->addWidget(new QWidget()); // Côté droit -> Informations (Details)
}