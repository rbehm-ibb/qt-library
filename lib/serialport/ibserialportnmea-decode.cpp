// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#include "ibserialportnmea.h"

/// \file ibserialportnmea-decode.cpp has been split off from ibserialportnmea.cpp
///
/// It contains the static NMEA decode utility functions.

QTime IBSerialPortNmea::toTime(const QByteArray &time)
{
	int h, m, s;
	if (time.isEmpty())
	{
		return QTime();	// invalid
	}
	int rc = sscanf(time.constData(), "%02d%02d%02d", &h, &m, &s);
	if (rc != 3)
	{
		return QTime();	// invalid
	}
	return QTime(h, m, s);	// will test for valid parms
}

QDate IBSerialPortNmea::toDate(const QByteArray &str)
{
	return toDate(str.left(2), str.mid(2, 2), str.mid(4,2));
}

QDate IBSerialPortNmea::toDate(const QByteArray &day, const QByteArray &mon, const QByteArray &year)
{
	if (day.isEmpty() || mon.isEmpty() || year.isEmpty())
	{
		return QDate();	// invalid
	}
	bool ok;
	int y = year.toUInt(&ok);
	if (! ok)
	{
		return QDate(); //invalid
	}
	return 	QDate(y + 2000, mon.toUInt(), day.toUInt());	// will test for valid parms
}

double IBSerialPortNmea::toCoord(const QByteArray &num, const QByteArray &hemi)
{
	if (num.isEmpty() || hemi.isEmpty()) return 0;
	int idp = num.indexOf('.');
	if (idp < 4)
	{
		return qQNaN();
	}
	bool ok;
	double deg = num.left(idp-2).toDouble(&ok);
	if (! ok)
	{
		return qQNaN();
	}
	double min = num.mid(idp-2).toDouble(&ok);
	if (! ok)
	{
		return qQNaN();
	}
	double x = deg + min / 60.0;
	char c = hemi[0];
	switch(c)
	{
	case 'E':
	case 'N':
		return x;
	case 'W':
	case 'S':
		return -x;
	// else fall thru and return error
	}
	return qQNaN();
}
