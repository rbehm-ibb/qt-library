// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * File: main.cpp
// * created 2/7/2015 by behm
// ******************************************************

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("CsvModel");
	app.setApplicationVersion("V0.1");
	app.setOrganizationDomain("rbehm.de");
	app.setOrganizationName("R.Behm");
	QCommandLineParser parser;
	parser.setApplicationDescription(app.applicationName());
	parser.addHelpOption();
	parser.addVersionOption();

//	QCommandLineOption nameOption(QStringList() << "n" << "name", "Name of CommServer ", "gps", "gps");
//	parser.addOption(nameOption);
	parser.process(app);

	MainWindow mw;
	mw.show();
	return app.exec();
}
