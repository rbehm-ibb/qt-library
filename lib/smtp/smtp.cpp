// **********************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// **********************************************************

#include "smtp.h"

static bool verbose = true;

QString encodeBase64(QByteArray in);

Smtp::Smtp(int port)
	: m_port(port)
	, m_dns(0)
{
	if (verbose) qDebug() << Q_FUNC_INFO << port;
	m_state = Init;
}


Smtp::~Smtp()
{
	if (verbose) qDebug() << Q_FUNC_INFO;
}

void Smtp::send(const SmtpMail &m)
{
	m_mail = m;

	m_socket = new QTcpSocket(this);
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));

	QString dest = m_mail.to().section('@', 1);
	if (dest != "localhost" && ! dest.startsWith("192.168."))
	{
		if (verbose) qDebug() << Q_FUNC_INFO << "DNS lookup for" << dest;
		m_dns = new QDnsLookup(QDnsLookup::MX, dest, this);
		connect(m_dns, &QDnsLookup::finished, this, &Smtp::dnsFinished);
		m_dns->lookup();
	}
	else
	{
// 		qDebug() << "Connecting direct to " << dest;
		m_socket->connectToHost(dest, m_port);
		m_outStream.setDevice(m_socket);
	}
	return;
}

void Smtp::connected()
{
	if (verbose) qDebug() << "Connected to" << m_socket->peerName();
}

void Smtp::socketError(QAbstractSocket::SocketError e)
{
	if (e != QAbstractSocket::RemoteHostClosedError)
	{
		qDebug() << Q_FUNC_INFO << m_socket->errorString();
	}
	deleteLater();
}

void Smtp::dnsFinished()
{
	qDebug() << Q_FUNC_INFO;
	if (m_dns->error() != QDnsLookup::NoError)
	{
		qWarning() << Q_FUNC_INFO << m_dns->name() << m_dns->errorString();
		deleteLater();
		return;
	}

	// Handle the results.
	foreach (const QDnsMailExchangeRecord &record, m_dns->mailExchangeRecords())
	{
		if (verbose) qDebug() << "Connecting" << record.exchange();
		m_socket->connectToHost(record.exchange(), m_port);
		m_outStream.setDevice(m_socket);
		return;
	}
	qWarning() << Q_FUNC_INFO << "no MX found for" << m_mail.to();
	deleteLater();
}

void Smtp::readyRead()
{
	// SMTP is line-oriented
	while (m_socket->canReadLine())
	{
		QByteArray resp = m_socket->readLine();
		resp.replace("\n", "");
		resp.replace("\r", "");
		if ((resp.length() > 3 && resp[3] != ' '))
			continue;
		if (verbose) qWarning() << Q_FUNC_INFO << m_state << "rx" << resp;
		char first = resp[0];

		if (m_state == Init && first == '2')
		{
			// banner was okay, let's go on
			m_outStream << "HELO " + QHostInfo::localHostName() + "\r\n";
			m_state = Mail;
		}
		else if (m_state == Mail && first == '2')
		{
			// HELO response was okay (well, it has to be)
			m_outStream << "MAIL FROM: <" << m_mail.from() << ">\r\n";
			if (verbose) qWarning() << Q_FUNC_INFO << "MAIL FROM:" << m_mail.from();
			m_state = Rcpt;
		}
		else if (m_state == Rcpt && first == '2')
		{
			m_outStream << "RCPT TO: <" << m_mail.to() << ">\r\n";
			if (verbose) qWarning() << Q_FUNC_INFO << "RCPT TO:" << m_mail.to();
			m_state = Data;
		}
		else if (m_state == Data && first == '2')
		{
			m_outStream << "DATA\r\n";
			if (verbose) qWarning() << Q_FUNC_INFO << "DATA";
			m_state = Body;
		}
		else if (m_state == Body && first == '2')
		{
			qWarning() << Q_FUNC_INFO << "strange response in state Body, ignored" << resp;
		}
		else if (m_state == Body && first == '3')
		{
			m_outStream << m_mail << "\r\n.\r\n";
			if (verbose) qDebug() << Q_FUNC_INFO /*<< m_mail*/ << "\r\n.\r\n";
			m_state = Quit;
		}
		else if (m_state == Quit && first == '2')
		{
			m_outStream << "QUIT\r\n";
			if (verbose) qWarning() << Q_FUNC_INFO << "QUIT";
			// here, we just close.
			m_state = Close;
			if (verbose) qWarning() << Q_FUNC_INFO << "Message sent";
		}
		else if (m_state == Close)
		{
			deleteLater();
			return;
		}
		else
		{
			qWarning() << Q_FUNC_INFO << "Unexpected reply from SMTP server: State = " << int(m_state) << endl;
			m_state = Close;
			deleteLater();
		}

	}

	m_outStream.flush();
}
