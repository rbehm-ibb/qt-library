// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 9/7/2018 by behm
// ******************************************************

#ifndef IPCTCPSOCKET_H
#define IPCTCPSOCKET_H


class IpcTcpSocket : public QTcpSocket
{
	Q_OBJECT
	friend class IpcTcpServer;
public:
	enum State	/// for internal state machine
	{
		WaitForSignature,	///< idle, we are waiting for the signature
		WaitForSize,		///< got sig, wait for size
		WaitForData		///< get size wait for data
	};
	Q_ENUM(State)
	explicit IpcTcpSocket(QObject *parent = 0);
	~IpcTcpSocket();
	bool isConnected() const { return state() == QTcpSocket::ConnectedState; }	///< are we connected to the server?
	QHostAddress host() const { return isConnected() ? peerAddress() : QHostAddress(); }
	quint16 port() const { return isConnected() ? peerPort() : 0; }

signals:
	void dataRxd(int signature, const QByteArray data);
	void connectStateChanged(bool connected);			///< the connection state has changed

public slots:
	void send(quint8 signature, const QByteArray data);	///< send data with out using stream
	void connectToHost(QHostAddress host, quint16 port);	///< tries to (re)connect to the server
protected slots:
private slots:
	void readyReadSlot();					///< something has been received
	void connectedSlot();					///< we are connected
	void disconnectedSlot();				///< we are disconnected
	void timeout();
	void stateChanged(QAbstractSocket::SocketState socketState);
protected:
	QTimer * m_watchDogTimer;	///< used to watch for any reception and signal timeout
private:
	IpcTcpSocket(quintptr socketDescriptor, QObject *parent = 0);
	State m_rxState;	///< for internal state machine
	qint8 m_sig;		///< received signature
	qint16 m_size;		///< received size
	QByteArray m_rxdData;	///< to collect received data
	QHostAddress m_host;
	quint16 m_port;
};

#endif // IPCTCPSOCKET_H
