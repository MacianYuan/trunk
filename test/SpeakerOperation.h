#ifndef SPEAKEROPERATION_H
#define SPEAKEROPERATION_H

#include <QThread>
#include <QProcess>

class SpeakerOperation: public QThread 
{
    Q_OBJECT
public:
    explicit SpeakerOperation(QObject* parent = 0);
    ~SpeakerOperation();

	void run();
	void stop();
	void play();

private:
	bool m_Status{ false };
	QProcess* m_SoundProcess{ nullptr };
};

#endif //SPEAKEROPERATION_H
