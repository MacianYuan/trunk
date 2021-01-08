#include "mainwindow.h"
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtGui/QWSServer>
#include <QtCore/QTextCodec>

#include <log4cplus/ndc.h>
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>
using namespace log4cplus;
using namespace log4cplus::helpers;

int main(int argc, char *argv[])
{
	SharedAppenderPtr _append(new RollingFileAppender("/tmp/audioTest.log", 3 * 1024 * 1024, 5));
	_append->setName("QT");
	std::string pattern("%D{%m/%d/%y %H:%M:%S} : %m [%F:%L]%n");
	_append->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
	Logger::getRoot().addAppender(_append);
	Logger root = Logger::getRoot();

	QApplication app(argc, argv);
	//app.setQuitOnLastWindowClosed(false);

	QWSServer::setCursorVisible(false);

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));


	QFont font("WenQuanYi Micro Hei");
	font.setPixelSize(18);
	app.setFont(font);

	// 设置工作目录为应用程序所在目录
	QDir::setCurrent(QCoreApplication::applicationDirPath());

    MainWindow w;
	w.showFullScreen();

    return app.exec();
}
