#include <iostream>
#include <QtGui/QMainWindow>
#include <QtGui/QStringListModel>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    initMusicFile();
    m_SoundProcess = new QProcess(this);

    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listView, SIGNAL(doubleclicked(
                                         const QModelIndex &)), this, SLOT(doubleclickedListView(
                                                                                   const QModelIndex &)));

    connect(ui->horizontalSliderA, SIGNAL(valueChanged(int)), this, SLOT(soundSizeChange(int)));
    ui->horizontalSliderA->setValue(50);

    connect(ui->pushButtonBefore, SIGNAL(clicked()), this, SLOT(playBefore()));
    connect(ui->pushButtonPlay, SIGNAL(clicked()), this, SLOT(playCurrent()));
    connect(ui->pushButtonPause, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(ui->pushButtonAfter, SIGNAL(clicked()), this, SLOT(playAfter()));


    QModelIndex index = ui->listView->model()->index(0, 0);
    ui->listView->setCurrentIndex(index);
}

MainWindow::~MainWindow() {
    delete ui;
}

QFileInfoList MainWindow::GetFileList(QString path) {
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);


    for (int i = 0; i != folder_list.size(); i++) {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = GetFileList(name);
        file_list.append(child_file_list);
    }

    return file_list;
}

void MainWindow::initMusicFile() {
    QFileInfoList mf = GetFileList(QString("/storage/sd0/music"));
    QStringList mfl;

    for (int i = 0; i < mf.size(); ++i) {
        auto f = mf.at(i);
        mfl << f.fileName();
    }

    QStringListModel *model = new QStringListModel(mfl);
    ui->listView->setModel(model);
}

void MainWindow::play(QString fileName) {
    if (QProcess::NotRunning != m_SoundProcess->state()) {
        m_SoundProcess->kill();
        m_SoundProcess->waitForFinished(3000);
    }
    QString p("/storage/sd0/music");
    m_SoundProcess->start("madplay", QStringList() << (p + "/" + fileName));
}

void MainWindow::stop() {
    if (QProcess::NotRunning != m_SoundProcess->state()) {
        m_SoundProcess->kill();
        m_SoundProcess->waitForFinished(3000);
    }
}

void MainWindow::playBefore() {
    changeIndex(false);
    std::cout << "play before" << std::endl;
    playCurrent();
}

void MainWindow::playCurrent() {
    QModelIndex index = ui->listView->currentIndex();
    mCurrentPlay = index.row();

    QString file = ui->listView->model()->data(index).toString();
    std::cout << "play current:" << file.toStdString() << std::endl;

    play(file);
}

void MainWindow::playPause() {
    std::cout << "play pause" << std::endl;
    stop();
}

void MainWindow::playAfter() {
    changeIndex(true);
    std::cout << "play after" << std::endl;
    playCurrent();
}

void MainWindow::changeIndex(bool st) {
    int v = mCurrentPlay + (false == st ? -1 : 1);
    v = (v < 0 ? 0 : v);

    int maxRowCount = ui->listView->model()->rowCount();
    v = (v >= maxRowCount ? (maxRowCount - 1) : v);
    mCurrentPlay = v;

    QModelIndex index = ui->listView->model()->index(v, 0);
    ui->listView->setCurrentIndex(index);
}

void MainWindow::soundSizeChange(int value) {
    int volume_write = ui->horizontalSliderA->value();
    //ui->horizontalSliderA->setValue();

    int HEAD_PHONE_MAX = 127;
    int PCM_MAX = 192;
    int headPhoneVol = 0;
    int pcmVol = 0;

    if (volume_write <= 0) {
        headPhoneVol = 0;
        pcmVol = 0;
    } else if (volume_write > 100) {
        headPhoneVol = HEAD_PHONE_MAX;
        pcmVol = PCM_MAX;
    } else {
        headPhoneVol =
                (int) ((volume_write * 1.0 / 100) * (HEAD_PHONE_MAX - HEAD_PHONE_MAX * 0.8)) + HEAD_PHONE_MAX * 0.8;
        pcmVol = (int) ((volume_write * 1.0 / 100) * (PCM_MAX - PCM_MAX * 0.8)) + PCM_MAX * 0.8;
    }

    char temp[128] = "amixer -c 0 sset '%s',0 %d,%d";
    QString cmd11 = tr("amixer -c 0 sset '%s',0 %d,%d");

    QString info1 = QString("amixer -c 0 sset '%1',0 %2,%3").arg("Headphone").arg(headPhoneVol).arg(headPhoneVol);
    QString info2 = QString("amixer -c 0 sset '%1',0 %2,%3").arg("PCM").arg(pcmVol).arg(pcmVol);

    QProcess p;
    p.start(info1);
    p.waitForFinished();
    p.start(info2);
    p.waitForFinished();
}

void MainWindow::doubleclickedListView(QModelIndex pos) {
    int row = pos.row();
    std::cout << "row is : " << row << std::endl;
}

