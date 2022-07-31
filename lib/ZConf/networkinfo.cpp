// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/13/2022 by behm
// ******************************************************

#include "networkinfo.h"
#include <sys/sysinfo.h>

QHostAddress NetworkInfo::myAddress()
{
	QHostAddress ip1;
	foreach (const QNetworkInterface &ni, QNetworkInterface::allInterfaces())
	{
		if ((ni.flags() & QNetworkInterface::IsRunning) && ! (ni.flags() & QNetworkInterface::IsLoopBack))
		{
//			qDebug() << Q_FUNC_INFO <<ni.name() << ni.humanReadableName() << ni.type() << ni.flags() << ni.isValid();
			const QList<QNetworkAddressEntry> al  = ni.addressEntries();
			foreach (const QNetworkAddressEntry &ae, al)
			{
//				qDebug() << Q_FUNC_INFO << ae.ip() << ae.isPermanent();
				if (ae.ip().protocol() == QAbstractSocket::IPv4Protocol)
				{
					ip1 = ae.ip();
					if (ni.name().contains("wlan"))
					{
						ip1 = ae.ip();
					}
					else
					{
						return ae.ip();
					}
				}
			}
		}
	}
	return ip1;
}

QDateTime NetworkInfo::startTime()
{

	struct sysinfo sinfo;
	int rc = sysinfo(&sinfo);
	QDateTime st = QDateTime::currentDateTime();
	if (rc ==  0)
	{
		st = st.addSecs(-sinfo.uptime);
	}
	return st;
}
