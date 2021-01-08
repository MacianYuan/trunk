#include "deviceinfo/tq2440.h"
#include "SpeakerOperation.h"
#include <iostream>

SpeakerOperation::SpeakerOperation(QObject* parent /* = 0 */) : QThread(parent)
{
	m_Status = false;
	m_SoundProcess = new QProcess(this);
}

SpeakerOperation::~SpeakerOperation()
{
	m_Status = false;
}

void SpeakerOperation::run()
{
	m_Status = true;
	while (true == m_Status) {
		if (QProcess::NotRunning == m_SoundProcess->state()) {
			break;
		}
		QThread::sleep(1);
	}
}

void SpeakerOperation::stop()
{
	m_Status = false;
}

void SpeakerOperation::play()
{
	QStringList param;
	param << "./media/end.mp3";
	if (QProcess::NotRunning != m_SoundProcess->state()) {
		m_SoundProcess->kill();
		m_SoundProcess->waitForFinished(3000);
	}
	m_SoundProcess->start("./madplay", param);
}
