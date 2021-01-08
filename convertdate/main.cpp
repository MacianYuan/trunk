#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QDebug>
QString formatIdString(quint64 id) {
    QDate date = QDate(1, 1, 1).addDays((id >> 32) - 364);
    uint instrument = (id >> 16) & 0xffff;
    uint index = id & 0xffff;
    return QString("%1-%2-%3").arg(date.toString("yyMMdd")).
            arg(instrument, 4, 10, QLatin1Char('0')).arg(index, 4, 10, QLatin1Char('0'));
}

int main ()
{
	qDebug() << formatIdString(3165051594801151);
return 0;
}
