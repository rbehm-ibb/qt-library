// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef TCPCOMMSERVER_H
#define TCPCOMMSERVER_H

#include "project.h"
#include "tcpcommsocket.h"
// #include "commsocket.h"

//// \brief Server for communication between processes
/// It uses QLocalSocket inside and allow transfer of data blocks
/// It mimics QCopChannel from Qt/E

class TcpCommServer : public QTcpServer
{
	Q_OBJECT
public:
	/// creates a local server listinging for connections on name
	TcpCommServer(quint16 port, QObject *parent = 0);
	~TcpCommServer();
	bool isConnected() { return ! sockets().isEmpty(); }
	/// used to send data
//	void send(CommSocket::CommSig signature) { send(signature, QByteArray()); }
signals:
	void connectedSignal();
	void disconnectedSignal();
	/// emitted when new data has arrived
	void dataRxd(TcpCommSocket::CommSig signature, const QByteArray data);
public slots:
	void send(TcpCommSocket::CommSig signature, const QByteArray data);
	void connectedOther();
	void disconnectedOther();
protected slots:
	void disconnectedSlot();
protected:
	/// called when a new connection comes in
	/// will create a CommSocket if first and set up signals/slots
	void incomingConnection(qintptr socketDescriptor);
	QList<TcpCommSocket*> sockets() const;
};

#endif
