// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/22/2017 by behm
// ******************************************************

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Scale testing");
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
#if 1
	QString dsn(":/scalegauge.css");
	QFile fcss(dsn);
	if (fcss.open(QFile::ReadOnly))
	{
		QTextStream scss(&fcss);
		app.setStyleSheet(scss.readAll());
	}
	else
	{
		qWarning() << Q_FUNC_INFO << fcss.fileName() << fcss.errorString();
	}
#endif
	MainWindow mw;
	mw.show();
	return app.exec();

}
