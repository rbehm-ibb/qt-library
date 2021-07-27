// ******************************************************
// * copyright (C) 2021 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/23/2021 by behm
// ******************************************************

#include "telnetserver.h"
#include "telnetsocket.h"
#include "config.h"

TelnetServer::TelnetServer(QObject *parent)
	: QTcpServer(parent)
	, m_maxClients(10)
	, m_timeout(10)
{
	QHostAddress::SpecialAddress type = QHostAddress::Any;
	QString confType = Config::stringValue("server/type", "anyipv4").toLower();
	if (confType == "any")
	{
		type = QHostAddress::Any;
	}
	else if (confType == "anyipv4")
	{
		type = QHostAddress::AnyIPv4;
	}
	else if (confType == "anyipv6")
	{
		type = QHostAddress::AnyIPv6;
	}
	quint16 port = Config::intValue("server/port", 5400);
	if (! listen(type, port))
	{
		qWarning() << Q_FUNC_INFO << port << errorString();
	}
}


void TelnetServer::incomingConnection(qintptr handle)
{
	if (findChildren<TelnetSocket*>().count() < m_maxClients)
	{
		TelnetSocket *sock = new TelnetSocket(handle, this);
		connect(sock, &TelnetSocket::lineRxd, this, &TelnetServer::lineRxd);
		connect(sock, &TelnetSocket::closedClient, this, &TelnetServer::closedClient);
		connect(this, &TelnetServer::sendSig, sock, &TelnetSocket::send);
		sock->setWatchtime(m_timeout);
		emit newClient(sock->peer());
	}
}



void TelnetServer::setMaxClients(int newMaxClients)
{
	m_maxClients = newMaxClients;
}

QList<QHostAddress> TelnetServer::clients() const
{
	QList<QHostAddress> res;
	foreach (const TelnetSocket *client, findChildren<TelnetSocket*>())
	{
		if (client->isOpen())
		{
			res += client->peer();
		}
	}
	return res;
}

void TelnetServer::setWatchtime(int sec)
{
	m_timeout = sec *  1000;
	foreach (TelnetSocket *client, findChildren<TelnetSocket*>())
	{
		if (client->isOpen())
		{
			client->setWatchtime(m_timeout);
		}
	}
}

void TelnetServer::send(const QByteArray line)
{
	emit sendSig(line);
}
