#include <iostream>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceinfo/tq2440.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	m_CardReader = new CardReader(this);
	m_CardReader->setGuardInterval(1000);

	m_BeepOp = new BeepOperation(this);
	m_SpeakerOp = new SpeakerOperation(this);
	m_SoundProcess = new QProcess(this);

    ui->setupUi(this);
	connect(ui->pushButtonBeepOpen, SIGNAL(clicked()), this, SLOT(BeepOpen()));
	connect(ui->pushButtonBeepClose, SIGNAL(clicked()), this, SLOT(BeepClose()));
	connect(ui->pushButtonSpeakerOpen, SIGNAL(clicked()), this, SLOT(SpeakerOpen()));
	connect(ui->pushButtonSpeakerClose, SIGNAL(clicked()), this, SLOT(SpeakerClose()));
	connect(ui->horizontalSliderSoundSize, SIGNAL(valueChanged(int)), this, SLOT(SoundSize(int)));
	connect(ui->pushButtonCardOpen, SIGNAL(clicked()), this, SLOT(CardOpen()));
	connect(ui->pushButtonCardClose, SIGNAL(clicked()), this, SLOT(CardClose()));
	connect(ui->pushButtonScreenCalibration, SIGNAL(clicked()), this, SLOT(ScreenCalibration()));
	connect(ui->pushButtonScreenTest, SIGNAL(clicked()), this, SLOT(ScreenTest()));
	connect(ui->pushButtonWifiTest, SIGNAL(clicked()), this, SLOT(WifiTest()));
	connect(ui->pushButtonEthTest, SIGNAL(clicked()), this, SLOT(EthTest()));
	connect(m_CardReader, SIGNAL(cardDetected(uint)), this, SLOT(CardDetected(uint)));

	char * ptr = getenv("FTP_UPLOAD_ABLE");
	if (NULL == ptr || 0 == atol(ptr)) {
		ui->labelSDCardStatus->setText(tr("挂载失败"));
	} else if (NULL != ptr || 1 == atol(ptr)) {
		ui->labelSDCardStatus->setText(tr("挂载成功"));
	}

	CardOpen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BeepOpen()
{
	m_BeepOp->start();
}

void MainWindow::BeepClose()
{
	m_BeepOp->stop();
}

void MainWindow::SpeakerOpen()
{
	QStringList param;
	param << "./media/end.mp3";
	m_SoundProcess->start("./madplay", param);

	m_SpeakerOp->start();
}

void MainWindow::SpeakerClose()
{
	m_SpeakerOp->stop();
	if (QProcess::NotRunning != m_SoundProcess->state()) {
		m_SoundProcess->kill();
		m_SoundProcess->waitForFinished(3000);
	}
}

void MainWindow::SoundSize(int)
{
	int vol = ui->horizontalSliderSoundSize->value();
	volume(vol * 20);
}

void MainWindow::CardOpen()
{
	m_CardReader->open();
}

void MainWindow::CardClose()
{
	m_CardReader->close();
}

void MainWindow::CardDetected(uint serial)
{
	QMessageBox::information(this, tr("读取到的卡号"), QString("%1").arg(serial));
}

void MainWindow::ScreenCalibration()
{
	QProcess::startDetached("ts_calibrate");
	exit(0);
}

void MainWindow::ScreenTest()
{
	QProcess::startDetached("ts_test");
	exit(0);
}

void MainWindow::WifiTest()
{

}

void MainWindow::EthTest()
{

}
