// ******************************************************
// * copyright (C) 2021 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/23/2021 by behm
// ******************************************************

#ifndef TELNETSERVER_H
#define TELNETSERVER_H

class TelnetSocket;

class TelnetServer : public QTcpServer
{
	Q_OBJECT
public:
	TelnetServer(QObject *parent = nullptr);

	int maxClients() const { return m_maxClients; }
	void setMaxClients(int newMaxClients);
	QList<QHostAddress> clients() const;
	void setWatchtime(int  sec);

signals:
	void lineRxd(const QByteArray line, const QHostAddress src);
	void sendSig(const QByteArray line);
	void newClient(const QHostAddress &peer);
	void closedClient(const QHostAddress &peer);
public slots:
	void send(const QByteArray line);
protected:
	virtual void incomingConnection(qintptr handle) override;
private:
	int m_maxClients;
	int m_timeout;
};

#endif // TELNETSERVER_H
