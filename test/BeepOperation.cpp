#include "deviceinfo/tq2440.h"
#include "BeepOperation.h"
#include <unistd.h>

BeepOperation::BeepOperation(QObject* parent /* = 0 */) : QThread(parent)
{
	m_Status = false;
}

BeepOperation::~BeepOperation()
{
	m_Status = false;
}

void BeepOperation::run()
{
	m_Status = true;
	while (true == m_Status) {
		play();
		::sleep(1);
	}
}

void BeepOperation::stop()
{
	m_Status = false;
}

void BeepOperation::play()
{
	beep(0, 1000);
}
