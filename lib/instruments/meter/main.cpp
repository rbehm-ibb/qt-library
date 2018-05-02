// ******************************************************
// * copyright 2015: (C) by Reinhardt Behm rbehm@hushmail.com
// ******************************************************

#include "project.h"
#include "testwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationDomain("rbehm.de");
	app.setOrganizationName("rbehm.de");
	app.setApplicationName("Meter-Test-Demo");
	app.setApplicationVersion("V0.1");

	QCommandLineParser parser;
	parser.setApplicationDescription(app.applicationName());
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption nameOption(QStringList() << "n" << "name", "Name of CommServer ", "gps", "gps");
	parser.addOption(nameOption);
	QCommandLineOption portOption(QStringList() << "p" << "port", "Port of TcpCommServer ", "4242", "4242");
	parser.addOption(nameOption);
	parser.addOption(portOption);
	parser.process(app);
	QString name = parser.value(nameOption);
	quint16 port = parser.value(portOption).toUInt();

	TestWindow tw;
	tw.show();
	return app.exec();
}
