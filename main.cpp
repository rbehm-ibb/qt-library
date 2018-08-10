// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * File: main.cpp
// * created 2/7/2015 by behm
// ******************************************************

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("LED testing");
	app.setApplicationVersion("V0.1");
	app.setOrganizationDomain("avioscout.net");
	app.setOrganizationName("R.Behm");
	app.setProperty("copyright-icon", ":/logo/ibb-logo");
	{
		QCommandLineParser parser;
		parser.setApplicationDescription(app.applicationName());
		parser.addHelpOption();
		parser.addVersionOption();
		parser.process(app);
		QCommandLineOption nameOption(QStringList() << "n" << "name", "Name of CommServer ", "gps", "gps");
		parser.addOption(nameOption);
	}

	MainWindow mw;
	mw.show();
	return app.exec();
}
