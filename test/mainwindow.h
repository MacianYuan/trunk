#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BeepOperation.h"
#include "SpeakerOperation.h"
#include "../controller/cardreader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void BeepClose();
	void BeepOpen();
	void SpeakerOpen();
	void SpeakerClose();
	void SoundSize(int);
	void CardOpen();
	void CardClose();
	void CardDetected(uint serial);
	void ScreenCalibration();
	void ScreenTest();
	void WifiTest();
	void EthTest();

private:
    Ui::MainWindow *ui;
	BeepOperation* m_BeepOp{ nullptr };
	SpeakerOperation* m_SpeakerOp{ nullptr };
	QProcess* m_SoundProcess{ nullptr };
	CardReader* m_CardReader{ nullptr };
	int m_CardGuard{ 2000 };
};

#endif // MAINWINDOW_H
