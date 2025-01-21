#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "audioplayer.h"
#include "fileloader.h"
#include <QTimer>
#include <deque>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onplayclicked();
    void onpauseclicked();
    void onstopclicked();
    void oncellclicked(int row, int col);
    void onvolumemoved(int slider_value);
    void updateDurationSlider();
    void updateDurationLabel();
    void ondurationreleased();
    void ondurationpressed();
    void onloaderclicked();
    void onshuffleclicked(bool checked);
    void onloopclicked(bool checked);
    void updatequeue();
    void addtoqueue(int row);
    void onclearclicked();
    void onqueuecellclicked(int row, int col);
    void onnextclicked();
    void onmuteclicked(bool checked);

public:
    Ui::MainWindow *ui;
    AudioPlayer audioplayer;
    FileLoader fileloader;
    QTimer timer;
    bool slider_pressed = false;
    bool loop_active = false;
    bool shuffle_active = false;
    int current_row;
    std::deque<int> song_queue;

};
#endif // MAINWINDOW_H
