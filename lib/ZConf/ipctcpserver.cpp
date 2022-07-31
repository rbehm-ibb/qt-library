// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 9/7/2018 by behm
// ******************************************************

#include "ipctcpserver.h"
#include "ipctcpsocket.h"
#include "modelsynchout.h"

IpcTcpServer::IpcTcpServer(quint16 port, QObject *parent)
	: QTcpServer(parent)
{
	if (! listen(QHostAddress::Any, port))
	{
		qWarning() << Q_FUNC_INFO << "no listen on" << port << errorString();
	}
	qDebug() << Q_FUNC_INFO << port << isListening();
}

IpcTcpServer::~IpcTcpServer()
{
	qDebug() << Q_FUNC_INFO << isListening();
	close();
}

void IpcTcpServer::send(int signature, const QByteArray data)
{
	ModelSynchOut::SynchCmd sig = ModelSynchOut::SynchCmd(signature);
//	qDebug() << Q_FUNC_INFO << sig << data.size() << data.toHex();
	emit sendPacket(sig, data);	// goes to all sockets
}

void IpcTcpServer::disconnectedSlot()
{
	IpcTcpSocket * socket = qobject_cast<IpcTcpSocket *>(sender());
	if (socket)
	{
		socket->deleteLater();
	}
//	qDebug() << Q_FUNC_INFO << serverName() << fullServerName();
	emit disconnectedSignal();
}

void IpcTcpServer::incomingConnection(qintptr handle)
{
	qDebug() << Q_FUNC_INFO;
	IpcTcpSocket * socket = new IpcTcpSocket(handle, this);
	connect(socket, &IpcTcpSocket::disconnected, this, &IpcTcpServer::disconnectedSlot);
	connect(socket, &IpcTcpSocket::dataRxd, this, &IpcTcpServer::dataRxd);
	connect(this, &IpcTcpServer::sendPacket, socket, &IpcTcpSocket::send);
	emit connectedSignal(socket);
}

QList<IpcTcpSocket *> IpcTcpServer::sockets() const
{
	return findChildren<IpcTcpSocket*>();
}
