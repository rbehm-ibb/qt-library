// ******************************************************
// * copyright (C) 2021 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/23/2021 by behm
// ******************************************************

#ifndef TELNETSOCKET_H
#define TELNETSOCKET_H

class StreamLineFilter;

class TelnetSocket : public QTcpSocket
{
	Q_OBJECT
public:
	explicit TelnetSocket(qintptr sock, QObject *parent = nullptr);
	~TelnetSocket();
	const QHostAddress &peer() const { return m_peer; }

	int watchtime() const { return m_watchtime; }
	void setWatchtime(int newWatchtime);

signals:
	void lineRxd(const QByteArray line, const QHostAddress src);
	void closedClient(const QHostAddress &peer);
public slots:
	void send(const QByteArray line);
private slots:
	void lineRxdSlot(StreamLineFilter *filter);
	void disconnectedSlot();
private:
	QHostAddress m_peer;
	StreamLineFilter *m_filter;
	int m_watchtime;
	QTimer *m_watch;
};

#endif // TELNETSOCKET_H
