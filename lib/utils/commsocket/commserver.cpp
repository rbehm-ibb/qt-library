// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#include "commserver.h"
#include "commsocket.h"

CommServer::CommServer(QString name, QObject *parent)
	: QLocalServer(parent)
	, m_name(name)
{
	QFile::remove("/tmp/" + CommSocket::prefix() + m_name);
	listen(CommSocket::prefix() + m_name);
	resetBuffer();
//	qDebug() << Q_FUNC_INFO << serverName() << fullServerName();
}

CommServer::~CommServer()
{
//	qDebug() << Q_FUNC_INFO << m_name;
	close();
	QFile::remove("/tmp/" + CommSocket::prefix() + m_name);
}

QList<CommSocket*> CommServer::sockets() const
{
	return findChildren<CommSocket*>();
}

void CommServer::send(int signature)
{
	emit sendPacket(signature, m_txData);	// goes to all sockets
	resetBuffer();
}

void CommServer::sendSlot(int signature, const QByteArray data)
{
	emit sendPacket(signature, data);	// goes to all sockets
}

void CommServer::resetBuffer()
{
	m_txBuffer.close();
	m_txStream.setDevice(0);
	m_txData.clear();
	m_txBuffer.setBuffer(&m_txData);
	m_txBuffer.open(QIODevice::WriteOnly);
	m_txStream.setDevice(&m_txBuffer);
}

void CommServer::incomingConnection(quintptr socketDescriptor)
{
//	qDebug() << Q_FUNC_INFO << m_name << socketDescriptor;
	CommSocket * socket = new CommSocket(socketDescriptor, m_name, this);
	connect(socket, &CommSocket::disconnected, this, &CommServer::disconnectedSlot);
	connect(socket, &CommSocket::dataRxd, this, &CommServer::dataRxd);
	connect(socket, &CommSocket::packetRxd, this, &CommServer::packetRxd);
	connect(this, &CommServer::sendPacket, socket, &CommSocket::sendDirect);
	emit connectedSignal();
}

void CommServer::disconnectedSlot()
{
	CommSocket * socket = qobject_cast<CommSocket *>(sender());
	if (socket)
	{
		socket->deleteLater();
	}
//	qDebug() << Q_FUNC_INFO << serverName() << fullServerName();
	emit disconnectedSignal();
}
