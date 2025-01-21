#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QRandomGenerator>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->volume->setRange(0, 100);
    ui->volume->setValue(100);

    connect(ui->play, &QPushButton::clicked, this, &::MainWindow::onplayclicked);
    connect(ui->pause, &QPushButton::clicked, this, &::MainWindow::onpauseclicked);
    connect(ui->stop, &QPushButton::clicked, this, &::MainWindow::onstopclicked);
    connect(ui->artistsongdisplay, &QTableWidget::cellDoubleClicked, this, &::MainWindow::oncellclicked);
    connect(ui->volume, &QSlider::valueChanged, this, &::MainWindow::onvolumemoved);
    connect(&timer, &QTimer::timeout, this, &::MainWindow::updateDurationSlider);
    connect(&timer, &QTimer::timeout, this, &::MainWindow::updateDurationLabel);
    connect(ui->durationslider, &QSlider::sliderReleased, this, &::MainWindow::ondurationreleased);
    connect(ui->durationslider, &QSlider::sliderPressed, this, &::MainWindow::ondurationpressed);
    connect(ui->loader, &QPushButton::clicked, this, &::MainWindow::onloaderclicked);
    connect(ui->shuffle, &QPushButton::clicked, this, &::MainWindow::onshuffleclicked);
    connect(ui->loop, &QPushButton::clicked, this, &::MainWindow::onloopclicked);
    connect(ui->clear, &QPushButton::clicked, this, &::MainWindow::onclearclicked);
    connect(ui->queuedisplay, &QTableWidget::cellDoubleClicked, this, &::MainWindow::onqueuecellclicked);
    connect(ui->next, &QPushButton::clicked, this, &::MainWindow::onnextclicked);
    connect(ui->mute, &QPushButton::clicked, this, &::MainWindow::onmuteclicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onplayclicked() {
    if(fileloader.getSongList().size() == 0) {
        return;
    }

    if(audioplayer.getStatus() != sf::Music::Status::Playing) {
        audioplayer.play();
        timer.start(100);
    }
}

void MainWindow::onpauseclicked() {
    audioplayer.pause();
}

void MainWindow::onstopclicked() {
    audioplayer.stop();
    ui->durationslider->setRange(0, audioplayer.getDuration());
    ui->durationslider->setValue(0);

    int offset = audioplayer.getDuration() - audioplayer.getOffset();
    int offset_min = offset / 60;
    int offset_sec = offset % 60;

    ui->durationlabel->setText(QString::number(offset_min, 'f', 0).rightJustified(2, '0') +
    ":" + QString::number(offset_sec, 'f', 0).rightJustified(2, '0'));
}

void MainWindow::oncellclicked(int row, int col) {
    if(col == 0 || col == 1) {
        QString qpath = ui->artistsongdisplay->item(row, 2)->text();
        wstring path = qpath.toStdWString();
        current_row = row;

        audioplayer.loadFile(path);

        ui->currentsong->clear();
        ui->currentsong->append(ui->artistsongdisplay->item(row, 0)->text() + " - " + ui->artistsongdisplay->item(row, 1)->text());
        ui->durationslider->setRange(0, audioplayer.getDuration());
        ui->durationslider->setValue(0);

        onplayclicked();
    } else if (col == 3) {
        addtoqueue(row);
    }


}

void MainWindow::onvolumemoved(int slider_value) {
    audioplayer.changeVolume(slider_value);
}

void MainWindow::updateDurationSlider() {

    if(audioplayer.getStatus() == sf::Music::Status::Playing && !slider_pressed) {
        ui->durationslider->setValue(audioplayer.getOffset());
    }
}

void MainWindow::updateDurationLabel() {
    int offset = audioplayer.getDuration() - audioplayer.getOffset();
    int offset_min = offset / 60;
    int offset_sec = offset % 60;

    if(audioplayer.getStatus() == sf::Music::Status::Playing) {
        ui->durationlabel->setText(QString::number(offset_min, 'f', 0).rightJustified(2, '0') +
        ":" + QString::number(offset_sec, 'f', 0).rightJustified(2, '0'));
    }

    if(audioplayer.getDuration() == audioplayer.getOffset()) {
        QRandomGenerator *randomGen = QRandomGenerator::global();

        if(loop_active) {
        }
        else if(!song_queue.empty()) {
            current_row = song_queue.front();
            song_queue.pop_front();

            updatequeue();
        }
        else if(shuffle_active) {
            int temp = current_row;
            while (current_row == temp) {
                current_row = randomGen->bounded(fileloader.getSongList().size());
            }
        } else ++current_row;
        if(current_row >= fileloader.getSongList().size()) current_row = 0;
        QString qpath = ui->artistsongdisplay->item(current_row, 2)->text();
        wstring path = qpath.toStdWString();

        audioplayer.loadFile(path);

        ui->currentsong->clear();
        ui->currentsong->append(ui->artistsongdisplay->item(current_row, 0)->text() + " - " + ui->artistsongdisplay->item(current_row, 1)->text());
        ui->durationslider->setRange(0, audioplayer.getDuration());
        ui->durationslider->setValue(0);

        onplayclicked();
    }
}

void MainWindow::ondurationreleased() {
    int current_value = ui->durationslider->value();
    audioplayer.setOffset(current_value);
    slider_pressed = false;
}

void MainWindow::ondurationpressed() {
    slider_pressed = true;
}

void MainWindow::onloaderclicked() {
    QString dir = QFileDialog::getExistingDirectory(nullptr, "Select your Music folder", "", QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty()) {
        fileloader.loadSongs(dir.toStdWString());
    }

    ui->artistsongdisplay->setRowCount(fileloader.getSongList().size());
    ui->artistsongdisplay->setColumnCount(4);

    ui->artistsongdisplay->setHorizontalHeaderItem(0, new QTableWidgetItem("Artist"));
    ui->artistsongdisplay->setHorizontalHeaderItem(1, new QTableWidgetItem("Song"));
    ui->artistsongdisplay->setHorizontalHeaderItem(3, new QTableWidgetItem("Q"));
    ui->artistsongdisplay->verticalHeader()->setVisible(false);

    ui ->artistsongdisplay->setColumnWidth(0, 120);
    ui->artistsongdisplay->setColumnWidth(1, 355);
    ui->artistsongdisplay->setColumnWidth(3, 20);

    int i = 0;
    for(auto song : fileloader.getSongList()) {
        ui->artistsongdisplay->setItem(i, 0, new QTableWidgetItem(QString(QString::fromStdWString(song.artist_name))));
        ui->artistsongdisplay->setItem(i, 1, new QTableWidgetItem(QString(QString::fromStdWString(song.song_name))));
        ui->artistsongdisplay->setItem(i, 2, new QTableWidgetItem(QString(QString::fromStdWString(song.path_name))));
        ui->artistsongdisplay->setItem(i, 3, new QTableWidgetItem("+"));
        ui->artistsongdisplay->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        ++i;
    }

    ui->artistsongdisplay->setColumnHidden(2, true);
    ui->artistsongdisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void MainWindow::onshuffleclicked(bool checked) {
    shuffle_active = !shuffle_active;
    if(shuffle_active) {
        ui->shuffle->setStyleSheet("background-color: green;");
    } else ui->shuffle->setStyleSheet("");
}

void MainWindow::onloopclicked(bool checked) {
    loop_active = !loop_active;
    if(loop_active) {
        ui->loop->setStyleSheet("background-color: green;");
    } else ui->loop->setStyleSheet("");
}

void MainWindow::updatequeue() {
    ui->queuedisplay->setRowCount(song_queue.size());
    ui->queuedisplay->setColumnCount(2);

    ui->queuedisplay->setHorizontalHeaderItem(0, new QTableWidgetItem("X"));
    ui->queuedisplay->setHorizontalHeaderItem(1, new QTableWidgetItem("Queued Songs"));

    ui->queuedisplay->setColumnWidth(0, 20);
    ui->queuedisplay->setColumnWidth(1, 120);

    int i = 0;
    for(auto num : song_queue) {
        ui->queuedisplay->setItem(i, 1, new QTableWidgetItem(QString(ui->artistsongdisplay->item(num, 1)->text())));
        ++i;
    }

    ui->queuedisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::addtoqueue(int row) {
    song_queue.push_back(row);

    updatequeue();
}

void MainWindow::onclearclicked() {
    while(!song_queue.empty()) {
        song_queue.pop_front();
    }

    updatequeue();
}

void MainWindow::onqueuecellclicked(int row, int col) {
    if(col == 0) {
        int i = 0;
        for(auto it = song_queue.begin(); it != song_queue.end(); ++it) {
            if(i == row) {
                song_queue.erase(it);
                break;
            }
            ++i;
        }
    }
    updatequeue();
}

void MainWindow::onnextclicked() {
    audioplayer.setOffset(audioplayer.getDuration());
}

void MainWindow::onmuteclicked(bool checked) {
    if(!checked) {
        audioplayer.changeVolume(0);
        ui->mute->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeMuted));
    }
    else {
        audioplayer.changeVolume(ui->volume->sliderPosition());
        ui->mute->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeHigh));
    }
}

