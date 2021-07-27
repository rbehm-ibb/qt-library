// ******************************************************
// * copyright (C) 2021 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/23/2021 by behm
// ******************************************************

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

class StreamLineFilter;

class ClientSocket : public QTcpSocket
{
	Q_OBJECT
public:
	explicit ClientSocket(QObject *parent = nullptr);
	~ClientSocket();
	void connectTo(QString host, quint16 port);
	void disconnect();
	const QHostAddress &peer() const { return m_peer; }

signals:
	void lineRxd(const QString line, const QHostAddress src);
	void state(bool connected);
public slots:
	void send(const QString line);
private slots:
	void lineRxdSlot(StreamLineFilter *that);
	void disconnectedSlot();
	void connectedSlot();
private:
	QHostAddress m_peer;
	StreamLineFilter *m_filter;
};

#endif // CLIENTSOCKET_H
