// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#include "project.h"
#include "tcpcommserver.h"
#include "tcpcommsocket.h"

TcpCommServer::TcpCommServer(quint16 port, QObject *parent)
	: QTcpServer(parent)
{
	if (! listen(QHostAddress::Any, port))
	{
		qWarning() << Q_FUNC_INFO << port << "error" << errorString();
	}
	qDebug() << Q_FUNC_INFO << port;
}

TcpCommServer::~TcpCommServer()
{
	// qDebug() << Q_FUNC_INFO << m_name;
	close();
}

QList<TcpCommSocket*> TcpCommServer::sockets() const
{
	return findChildren<TcpCommSocket*>();
}

void TcpCommServer::send(TcpCommSocket::CommSig signature, const QByteArray data)
{
	qDebug() << Q_FUNC_INFO << sockets().count() << signature << data.toHex();
	foreach (TcpCommSocket * socket, sockets())
	{
		socket->send(signature, data);
	}
}

void TcpCommServer::connectedOther()
{
	qDebug() << Q_FUNC_INFO;
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);
	int conn = 1;
	ds << conn;
	send(TcpCommSocket::StatusData, ba);
}

void TcpCommServer::disconnectedOther()
{
	qDebug() << Q_FUNC_INFO;
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);
	int conn = 0;
	ds << conn;
	send(TcpCommSocket::StatusData, ba);
}

void TcpCommServer::incomingConnection(qintptr socketDescriptor)
{
	TcpCommSocket * socket = new TcpCommSocket(socketDescriptor, this);
	connect(socket, &TcpCommSocket::disconnected, this, &TcpCommServer::disconnectedSlot);
	connect(socket, &TcpCommSocket::dataRxd, this, &TcpCommServer::dataRxd);
	emit connectedSignal();
	qDebug() << Q_FUNC_INFO << sockets().count() << socketDescriptor;
}

void TcpCommServer::disconnectedSlot()
{
	TcpCommSocket * socket = qobject_cast<TcpCommSocket *>(sender());
	if (socket)
		socket->deleteLater();
	qDebug() << Q_FUNC_INFO;
	emit disconnectedSignal();
}
