// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 9/7/2018 by behm
// ******************************************************

#ifndef IPCTCPSERVER_H
#define IPCTCPSERVER_H

class IpcTcpSocket;

class IpcTcpServer : public QTcpServer
{
	Q_OBJECT
public:
	explicit IpcTcpServer(quint16 port, QObject *parent = 0);
	~IpcTcpServer();
	bool isConnected() const { return ! sockets().isEmpty(); }	///< do we have any connection?
	void send(int signature, const QByteArray data);

signals:
	void connectedSignal(IpcTcpSocket *socket);			///< someone connected
	void disconnectedSignal();		///< someone disconnected
	void dataRxd(int signature, const QByteArray ds);

	void sendPacket(int signature, const QByteArray data);	///< intenally used to trigger sending in the connected CommSocket(s)

public slots:
protected slots:
	void disconnectedSlot();		///< internal to catch disconnects
private slots:
protected:
//	virtual void incomingConnection(quintptr socketDescriptor);
	QList<IpcTcpSocket *> sockets() const;	///< get all connected sockets
private:

	// QTcpServer interface
protected:
	virtual void incomingConnection(qintptr handle);
};

#endif // IPCTCPSERVER_H
