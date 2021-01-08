#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QProcess>
#include <QtGui/QMainWindow>
#include <QtCore/QFileInfoList>
#include <QtCore/QAbstractItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QFileInfoList GetFileList(QString path);
    void initMusicFile();
    void play(QString fileName);
    void stop();
    void changeIndex(bool st);

public slots:
    void playBefore();
    void playCurrent();
    void playPause();
    void playAfter();
    void soundSizeChange(int value);
    void doubleclickedListView(QModelIndex pos);

private:
    Ui::MainWindow *ui;
    QProcess* m_SoundProcess{ nullptr };
    int mCurrentPlay {0};
};

#endif // MAINWINDOW_H
