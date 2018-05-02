// **********************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// **********************************************************

#ifndef SMTP_H
#define SMTP_H

#include "smtpmail.h"

class Smtp : public QObject
{
	Q_OBJECT

public:
	Smtp(int port);
	~Smtp();
	void send(const SmtpMail &m);
//	void sendPure(QString to, QString from, QString m);
	enum State
	{
		Init,
		Mail,
		Rcpt,
		Data,
		Body,
		Quit,
		Close
	};
	Q_ENUM(State)

signals:
	void status(const QString &);

private slots:
	void readyRead();
	void connected();
	void socketError(QAbstractSocket::SocketError);
	void dnsFinished();

private:
	int m_port;
	SmtpMail m_mail;
	QTcpSocket *m_socket;
	QTextStream m_outStream;
	State m_state;
	QDnsLookup *m_dns;
};

#endif
