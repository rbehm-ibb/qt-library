// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#include "ibserialportnmea.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	app.setApplicationName("serial-test");
	app.setApplicationVersion("V0.1");
	app.setOrganizationDomain("rbehm@hushmail.com");
	app.setOrganizationName("R.Behm");
	QCommandLineParser parser;
	parser.setApplicationDescription(app.applicationName());
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption devOption(QStringList() << "d" << "device", "Port", "device");
	parser.addOption(devOption);
//	QCommandLineOption verboseOption(QStringList() << "t" << "verbose", "be verbose");
//	parser.addOption(verboseOption);
	parser.process(app);

	QString device = parser.value(devOption);
//	bool verbose = parser.isSet(verboseOption);
	IBSerialPortNmea port(device);
	if (port.ok())
	{	return app.exec();
	}
	else
	{
		return EXIT_FAILURE;
	}
}
