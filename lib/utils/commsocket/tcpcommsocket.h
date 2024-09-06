// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef TCPCOMMSOCKET_H
#define TCPCOMMSOCKET_H

// #include "project.h"

/// \brief Server for communication between processes
/// It uses QLocalSocket inside and allow transfer of data blocks
/// It mimics QCopChannel from Qt/E

class TcpCommSocket : public QTcpSocket
{
	Q_OBJECT
public:
	typedef quint16 CommSig;
	enum CommId	// these are the default ids, others may define more
	{
		NormalData, ErrorData, LogData, StatusData = 254
	};
	TcpCommSocket(QString ip, quint16 port, QObject *parent = 0);
	TcpCommSocket(quintptr socketDescriptor, QObject *parent = 0);
	~TcpCommSocket();
	/// are we connected to the server?
	bool isConnected() const { return state() == QTcpSocket::ConnectedState; }
signals:
	/// emitted when new data has arrived
	void dataRxd(quint16 signature, const QByteArray data);
public slots:
	/// send data with siganture
	void send(quint16 signature, const QByteArray data);
protected slots:
	void connectToServer();
	void readyReadSlot();
	void connectedSlot();
	void disconnectedSlot();
	void errorSlot(QAbstractSocket::SocketError socketError);
	void stateChangedSlot(QAbstractSocket::SocketState socketState);
protected:
	QString m_ip;
	quint16 m_port;
	enum State { WAIT_SIG, WAIT_SIZE, WAIT_DATA } m_rxState;
	quint16 m_sig;
	qint32 m_size;
	QByteArray m_rxdData;
	QTimer * m_reconnectTimer;
};

#endif
