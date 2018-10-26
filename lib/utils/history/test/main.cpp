// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/26/2018 by behm
// ******************************************************

#include "mainwindow.h"
#include "config.h"
#include "singleapp.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("file-history");
	app.setApplicationVersion("V0.1");
	app.setOrganizationDomain("avioscout.net");
	app.setOrganizationName("R.Behm");
	{
		const QString logo(":/logo/ibb-logo");
		app.setProperty("copyright-icon", logo);
		app.setWindowIcon(QIcon(":/list.png"));
	}
	{
		QCommandLineParser parser;
		parser.setApplicationDescription(app.applicationName());
		parser.addHelpOption();
		parser.addVersionOption();
		parser.process(app);
	}
	{
		QFile sf(":/styles.css");
		sf.open(QIODevice::ReadOnly);
		app.setStyleSheet(sf.readAll());
	}
	Config::loadDefaults();
	SingleApp sapp;
	MainWindow mw;
	QObject::connect(&sapp, &SingleApp::kill, &mw, &MainWindow::quit);
	mw.show();
	return app.exec();
}

