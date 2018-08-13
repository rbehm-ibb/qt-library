// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * File: main.cpp
// * created 2/7/2015 by behm
// ******************************************************

#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("resource-view");
	app.setApplicationVersion("V0.1");
	app.setOrganizationDomain("avioscout.net");
	app.setOrganizationName("R.Behm");
	{
		const QString logo(":/logo/ibb-logo");
		app.setProperty("copyright-icon", logo);
//		app.setWindowIcon(QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
		app.setWindowIcon(QIcon(logo));
	}
	{
		QCommandLineParser parser;
		parser.setApplicationDescription(app.applicationName());
		parser.addHelpOption();
		parser.addVersionOption();
		parser.process(app);
	}
	{
		QFile sf(":/style.css");
		sf.open(QIODevice::ReadOnly);
		app.setStyleSheet(sf.readAll());
	}
	Config::loadDefaults();
	MainWindow mw;
	mw.show();
	return app.exec();
}
