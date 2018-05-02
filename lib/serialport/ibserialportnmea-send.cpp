// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#include "ibserialportnmea.h"

/// \file ibserialportnmea-send.cpp has been split off from ibserialportnmea.cpp
///
/// It contains the NMEA send and encoding functions.

void IBSerialPortNmea::send(const QByteArray &a)
{
	sendStart(a);
	sendEnd();
}

void IBSerialPortNmea::sendStart(const QByteArray &a)
{
	m_chksum = 0;
	write("$");
	write(a);
	calcChksum(a);
//	qDebug() << Q_FUNC_INFO << a;
}

void IBSerialPortNmea::sendPart(const QByteArray &a)
{
	m_chksum ^= ',';
	write(",");
	calcChksum(a);
	write(a);
//	qDebug() << Q_FUNC_INFO << a;
}

void IBSerialPortNmea::sendLastPart(const QByteArray &a)
{
	sendPart(a);
//	qDebug() << Q_FUNC_INFO << a;
	sendEnd();
}

void IBSerialPortNmea::sendEnd()
{
	char ss[10];
	sprintf(ss, "*%02X\r\n", m_chksum);
	write(ss);
}

void IBSerialPortNmea::calcChksum(const QByteArray &a)
{
	for (int i = 0; i < a.length(); ++i)
	{
		m_chksum ^= a[i];
	}
}

void IBSerialPortNmea::sendXcoord(qreal x)
{
	char hemi = 'E';
	if (x < 0)
	{
		hemi = 'W';
		x = -x;
	}
	double xd;
	double xm = modf(x, &xd);
	char s[20];
	sprintf(s, "%02d%07.4f,%c", int(xd), xm*60., hemi);
	sendPart(s);
}

void IBSerialPortNmea::sendYcoord(qreal y)
{
	char hemi = 'N';
	if (y < 0)
	{
		hemi = 'S';
		y = -y;
	}
	double yd;
	double ym = modf(y, &yd);
	char s[20];
	sprintf(s, "%03d%07.4f,%c", int(yd), ym*60., hemi);
	sendPart(s);
}

void IBSerialPortNmea::sendCoord(const QPointF &p)
{
	sendYcoord(p.y());
	sendXcoord(p.x());
}

void IBSerialPortNmea::sendCoord(qreal x, qreal y)
{
	sendYcoord(y);
	sendXcoord(x);
}


void IBSerialPortNmea::sendTime(QTime time)
{
	char s[100];
	sprintf(s, "%02d%02d%02d.000",
		time.hour(),
		time.minute(),
		time.second()
		);
	sendPart(s);
}

qint64 IBSerialPortNmea::write(const char *data)
{
//	cout << data;
//	cout.flush();
	return QSerialPort::write(data);
}
