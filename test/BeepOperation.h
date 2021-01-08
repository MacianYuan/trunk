#ifndef BEEPOPERATION_H
#define BEEPOPERATION_H

#include <QThread>

class BeepOperation: public QThread 
{
    Q_OBJECT
public:
    explicit BeepOperation(QObject* parent = 0);
    ~BeepOperation();

	void run();
	void stop();
	void play();

private:
	bool m_Status{ false };
};

#endif //BEEPOPERATION_H
