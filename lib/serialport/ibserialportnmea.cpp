// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#include "ibserialportnmea.h"

IBSerialPortNmea::IBSerialPortNmea(const QString device, QObject *parent)
	: IBSerialPortLine(device, parent)
{
	setBol(QByteArray("$"));
	setEol(QByteArray("\n"));
	setIgnore(QByteArray("\r"));
	connect(this, &IBSerialPortLine::lineRxd, this, &IBSerialPortNmea::lineRxdSlot);
}

void IBSerialPortNmea::lineRxdSlot()
{
	while(canReadLine())
	{
		QByteArray line = readLine();	// w/o $ and \r\n
		int ipos = line.lastIndexOf('*');
		if (ipos >= 0)	// has chksum
		{
			QByteArray oline(line);
			uint checkSum = line.mid(ipos+1).toUInt(0, 16);	// convert as hex. Ignore error, will be catched later.
			line = line.left(ipos);
			quint8 csum = 0;
			for (int i = 0; i < line.length(); ++i)
			{
				csum ^= quint8(line.at(i));
			}
			if (csum != checkSum)
			{
				qWarning() << Q_FUNC_INFO << objectName() << "error checksum" << Qt::hex << checkSum << "calc" << csum << oline << Qt::dec;
				emit chksumError(line);
				continue;
			}
		}
		QList<QByteArray> v = line.split(',');
		if (v.count() >= 1)
		{
			emit nmeaRxd(v);
		}
	}
}
