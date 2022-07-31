// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/10/2022 by behm
// ******************************************************

#include "zconfclient.h"
#include "config.h"
#include "zconfdef.h"
#include "ipctcpsocket.h"
#include "modelsynchin.h"
#include "networkinfo.h"

ZconfClient::ZconfClient(QObject *parent)
	: QObject(parent)
	, m_synch(new ModelSynchIn(this))
	, m_table(new ZConfTable(this))
{
	m_synch->setModel(m_table);
	connect(m_synch, &ModelSynchIn::updated, this, &ZconfClient::updated);
}

void ZconfClient::open(uint16_t port, const QString id)
{
	if (m_udp)
	{
		m_udp->deleteLater();
	}
	if (m_tcp)
	{
		m_tcp->deleteLater();
	}
	m_udp =  new QUdpSocket(this);
	connect(m_udp, &QUdpSocket::stateChanged, this, &ZconfClient::stateChangedUdp);
	connect(m_udp, &QUdpSocket::readyRead, this, &ZconfClient::readyReadUdp);
	connect(m_udp, &QUdpSocket::errorOccurred, this, &ZconfClient::errorOccurred);
	bool rc = m_udp->bind(QHostAddress::AnyIPv4, ZConfDef::udpPortreply(), QAbstractSocket::DefaultForPlatform);
	if (! rc)
	{
		qWarning() << Q_FUNC_INFO << "bind" << rc << port << m_udp->errorString();
		emit rxd(m_udp->errorString());
		delete m_udp;
		return;
	}
	QString d(ZConfDef::magic() + ",?");
	d += ",ID:";
	if (! id.isNull())
	{
		d += id + ":";
	}
	d += QSysInfo::machineHostName();
	d += ",S:" + QSysInfo::productType() + " " + QSysInfo::productVersion();
	d += ",T:" + NetworkInfo::startTime().toString(Qt::ISODate);
//	qDebug() << Q_FUNC_INFO << port << d;
	QNetworkDatagram dg(d.toLocal8Bit(), QHostAddress::Broadcast, port);
	m_udp->writeDatagram(dg);
}

QString ZconfClient::hostInfo() const
{
	if (m_tcp)
	{
		return m_tcp->host().toString() + ":" +  QString::number(m_tcp->port());
	}
	return QString();
}

void ZconfClient::stateChangedUdp(QAbstractSocket::SocketState socketState)
{
//	qDebug() << Q_FUNC_INFO << socketState;
}

void ZconfClient::readyReadUdp()
{
//	qDebug() << Q_FUNC_INFO;
	while (m_udp->hasPendingDatagrams())
	{
		QNetworkDatagram dgram = m_udp->receiveDatagram();
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
		QStringList sl  = d.split(',');
		if (sl.first() != ZConfDef::magic())
		{
			emit rxd("Unknown response " + sl.first());
			return;
		}
		sl.removeFirst();
		foreach (const QString &s, sl)
		{
			const QString tag = s.section(':', 0, 0);
			const QString val = s.section(':', 1);
			if (tag == "TCP")
			{
				uint16_t port = val.toUInt();
				if (m_tcp)
				{
					m_tcp->deleteLater();
				}
				m_tcp = new IpcTcpSocket(this);
//				connect(m_tcp, &IpcTcpSocket::dataRxd, this, &ZconfClient::dataRxdIpc);
				connect(m_tcp, &IpcTcpSocket::connectStateChanged, this, &ZconfClient::connectState);
				m_synch->setSocket(m_tcp);
				m_tcp->connectToHost(dgram.senderAddress(), port);
			}
			else
			{
				emit rxd("unknown tag " + s);
			}
		}
//		qDebug() << Q_FUNC_INFO << dgram.senderAddress() << port;
	}
	m_udp->deleteLater();
}

void ZconfClient::stateChangedTcp(QAbstractSocket::SocketState socketState)
{
	Q_UNUSED(socketState)
//	qDebug() << Q_FUNC_INFO << socketState;
}

void ZconfClient::errorOccurred(QAbstractSocket::SocketError socketError)
{
	qDebug() << Q_FUNC_INFO << sender() << socketError;
}

//void ZconfClient::dataRxdIpc(int signature, const QByteArray data)
//{
//	ModelSynchIn::SynchCmd sig = ModelSynchIn::SynchCmd(signature);
//	qDebug() << Q_FUNC_INFO << sig << data.size() << data.toHex();

//}
