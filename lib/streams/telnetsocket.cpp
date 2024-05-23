// ******************************************************
// * copyright (C) 2021 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/23/2021 by behm
// ******************************************************

#include "telnetsocket.h"
#include "streamlinefilter.h"

TelnetSocket::TelnetSocket(qintptr sock, QObject *parent)
	: QTcpSocket(parent)
	, m_watchtime(10*1000)
	, m_watch(new QTimer(this))
{
	// qDebug() << Q_FUNC_INFO << sock;
	setSocketDescriptor(sock);
	m_peer = peerAddress();
	connect(this, &QTcpSocket::disconnected, this, &TelnetSocket::disconnectedSlot);
	connect(m_watch, &QTimer::timeout, this, &QTcpSocket::close);
	m_watch->start(m_watchtime);
	m_filter = new StreamLineFilter(this, this);
	connect(m_filter, &StreamLineFilter::lineRxd, this, &TelnetSocket::lineRxdSlot);
	qDebug() << Q_FUNC_INFO << m_peer;
	m_filter->setBol(QByteArray());
	// qDebug() <<Q_FUNC_INFO << m_filter->bol() << m_filter->eol();
}

TelnetSocket::~TelnetSocket()
{
	// qDebug() << Q_FUNC_INFO << m_peer;
	emit closedClient(m_peer);
}

void TelnetSocket::send(const QByteArray line)
{
	m_filter->sendLine(line);
}

void TelnetSocket::lineRxdSlot(StreamLineFilter *filter)
{
	// qDebug() << Q_FUNC_INFO << m_peer;
	while (filter->canReadLine())
	{
		m_watch->start(m_watchtime);
		const QByteArray line = filter->readLine();
		if (line == "!quit")
		{
			close();
		}
		emit lineRxd(line, m_peer);
	}
}

void TelnetSocket::disconnectedSlot()
{
	// qDebug() << Q_FUNC_INFO << peerAddress();
	deleteLater();
}

void TelnetSocket::setWatchtime(int newWatchtime)
{
	m_watchtime = newWatchtime*1000;
	m_watch->start(m_watchtime);
}
