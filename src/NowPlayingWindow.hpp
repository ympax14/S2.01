#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSplitter>
#include <QHBoxLayout>

class NowPlayingWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int WINDOW_WIDTH, WINDOW_HEIGHT;

    explicit NowPlayingWindow(QWidget *parent = nullptr);
    ~NowPlayingWindow() = default;

private:
    QHBoxLayout * const mainLayout;
    QSplitter * const splitter;
};
#endif // MAINWINDOW_HPP
