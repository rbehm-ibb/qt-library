// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/10/2022 by behm
// ******************************************************

#include "zconfserver.h"
#include "config.h"
#include "networkinfo.h"
#include "zconfdef.h"
#include "ipctcpserver.h"
#include "modelsynchout.h"

ZConfServer::ZConfServer(QObject *parent)
	: QObject(parent)
	, m_table(new ZConfTable(this))
{
	ZConfItem item;
	item.name = Config::stringValue("self/name");
	if (! item.name.isEmpty())
	{
		item.name = item.name + ":" + QSysInfo::machineHostName();
		item.serial = QSysInfo::productType() + " " + QSysInfo::productVersion(); // Config::stringValue("self/serial");
		item.port = 0;
		item.stamp = NetworkInfo::startTime();
		item.ip = NetworkInfo::myAddress().toString();
		m_table->addItem(item);
	}
	m_tcp = new IpcTcpServer(ZConfDef::tcpPort(), this);
	m_synch = new ModelSynchOut(m_tcp);
	m_synch->setModel(m_table);
}

void ZConfServer::open(uint16_t port)
{
	if (m_rx)
	{
		m_rx->deleteLater();
	}
	m_rx = new QUdpSocket(this);
	connect(m_rx, &QUdpSocket::stateChanged, this, &ZConfServer::stateChangedRx);
	connect(m_rx, &QUdpSocket::readyRead, this, &ZConfServer::readyReadRx);
	bool rc = m_rx->bind(QHostAddress::AnyIPv4, port);
	if (! rc)
	{
		qWarning() << Q_FUNC_INFO << "bind" << rc << port << m_rx->errorString();
		emit rxd(m_rx->errorString());
		delete m_rx;
		return;
	}
}

void ZConfServer::setMaxAge(int age)
{
	m_table->setMaxAge(age);
}

void ZConfServer::readyReadRx()
{
//	qDebug() << Q_FUNC_INFO;
	while (m_rx->hasPendingDatagrams())
	{
		QNetworkDatagram dgram = m_rx->receiveDatagram();
		QString d = QString::fromLocal8Bit(dgram.data());
		d = d.replace("\n", "");
		d = d.replace("\r", "");
		QHostAddress srcIp = dgram.senderAddress();
//		qDebug() << Q_FUNC_INFO << srcIp << d;
		emit rxd(QString("Rx %1:%2 -> %4:%5 <%3>")
				.arg(srcIp.toString())
				.arg(dgram.senderPort())
				.arg(d)
				.arg(dgram.destinationAddress().toString())
				.arg(dgram.destinationPort())
				);
		if (! d.startsWith(ZConfDef::magic()))
		{
			emit rxd("Bad magic rxd");
			return;
		}
		QStringList sl  = d.split(',');
//		qDebug() << Q_FUNC_INFO << sl;
		sl.removeFirst();	// skip  magic
		if (sl.first() == "?")
		{
			sl.removeFirst();	// skip  magic
			ZConfItem item;
			item.port = 0;
			item.ip = dgram.senderAddress().toString();
			item.name =  "????";
			foreach (const QString &s, sl)
			{
				if (s.startsWith("P:"))
				{
					item.port = s.midRef(2).toUInt();
				}
				else if (s.startsWith("S:"))
				{
					item.serial = s.mid(2);
				}
				else if (s.startsWith("ID:"))
				{
					item.name = s.mid(3);
				}
				else if (s.startsWith("T:"))
				{
					item.stamp = QDateTime::fromString(s.mid(2), Qt::ISODate);
				}
				else
				{
					emit rxd("Unknown tag: " +  s);
				}
			}
			m_table->addItem(item);
			QString resp(ZConfDef::magic() + ",TCP:%1");
			m_rx->writeDatagram(resp.arg(ZConfDef::tcpPort()).toLocal8Bit(), dgram.senderAddress(), dgram.senderPort());
			return;
		}
		if (sl.count() < 3)
		{
			emit rxd("Bad count");
			return;
		}
		ZConfItem item;
		item.port = 0;
		foreach (const QString &s, sl)
		{
			if (s.startsWith("P:"))
			{
				item.port = s.midRef(2).toUInt();
			}
			else if (s.startsWith("S:"))
			{
				item.serial = s.mid(2);
			}
			else if (s.startsWith("ID:"))
			{
				item.name = s.mid(3);
			}
			else
			{
				emit rxd("Unknown tag: " +  s);
			}
		}
		if ((item.port == 0) || item.name.isEmpty())
		{
			emit rxd("Tag missing");
		}
		else
		{
			item.stamp = QDateTime::currentDateTime();
			item.ip = srcIp.toString();
			m_table->addItem(item);
		}
	}
}

void ZConfServer::stateChangedRx(QAbstractSocket::SocketState socketState)
{
	qDebug() << Q_FUNC_INFO << socketState;
}
