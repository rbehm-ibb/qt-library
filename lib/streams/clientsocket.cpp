// ******************************************************
// * copyright (C) 2021 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/23/2021 by behm
// ******************************************************

#include "clientsocket.h"
#include "streamlinefilter.h"

ClientSocket::ClientSocket(QObject *parent)
	: QTcpSocket(parent)
	, m_filter(new StreamLineFilter(this, this))
{
//	qDebug() << Q_FUNC_INFO << m_peer;
	connect(m_filter, &StreamLineFilter::lineRxd, this, &ClientSocket::lineRxdSlot);
	connect(this, &QTcpSocket::connected, this, &ClientSocket::connectedSlot);
	connect(this, &QTcpSocket::disconnected, this, &ClientSocket::disconnectedSlot);
}

ClientSocket::~ClientSocket()
{
//	qDebug() << Q_FUNC_INFO << m_peer;
//	emit state(false);
}

void ClientSocket::connectTo(QString host, quint16 port)
{
	connectToHost(host, port);
}

void ClientSocket::send(const QString line)
{
	m_filter->sendLine(line.toLocal8Bit());
}

void ClientSocket::lineRxdSlot(StreamLineFilter *that)
{
	emit lineRxd(that->readLine(), m_peer);
}

void ClientSocket::disconnectedSlot()
{
//	qDebug() << Q_FUNC_INFO << peerAddress();
	emit state(false);
}

void ClientSocket::connectedSlot()
{
	m_peer = peerAddress();
	emit state(true);
}
