// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef COMMSERVER_H
#define COMMSERVER_H

#include "commsocket.h"

class CommSocket;

//// \brief Server for communication between processes. Uses CommSocket for the real work.
///
/// See also CommSocket for a deeper description how data is transferred.
///
/// Uses QLocalServer/QLocalSocket internally which is based on Unix domain sockets in Linux.
///
/// It uses QLocalSocket inside and allow transfer of data blocks.
/// \n It mimics QCopChannel from Qt/E
///
/// It can handle multiple connections. Sending happens to all connected CommSockets,
/// Repection from any is signalled without identification of which conncected CommSocket it came from.
/// \n When using multi-connections all except one socket should be listeners only.
///
/// The sending consists of putting data on the txStream() (via operator<<() )
/// and calling send when ready. The txStream is automatically cleared.
///
/// The receive interface acts thru the signals dataRxd() or packetRxd().
/// Both are emitted when a connected socket has something rxd.
/// packetRxd(int signature, QDataStream & ds) gives a readonly QDataStream, ready for use with any QDataStream::operator>>().
/// dataRxd(int signature, const QByteArray data) gives the just the data.
///

class CommServer : public QLocalServer
{
	Q_OBJECT
public:
	/// creates a local server listinging for connections on name
	CommServer(QString name, QObject *parent = nullptr);
	~CommServer();
	bool isConnected() { return ! sockets().isEmpty(); }	///< do we have any connection?
	QDataStream & txStream() { return m_txStream; }		///< used to collect data via operator<<(). \sa CommSocket
	void send(int signature = CommSocket::NormalData);	///< send the collected data. Resets buffer. \sa CommSocket
signals:
	void connectedSignal();			///< someone connected
	void disconnectedSignal();		///< someone disconnected
	/// \brief emitted when new data has arrived.
	///
	/// Both dataRxd() and packetRxd() are emitted
	/// \param signature of the rxd block
	/// \param data the raw data
	void dataRxd(int signature, const QByteArray data);
	/// \brief emitted when new data has arrived, for easy handling with >> operators.
	///
	/// Both dataRxd() and packetRxd() are emitted
	/// \param signature of the rxd block
	/// \param ds a readonly QDataStream. Can be used to extract data using >> operators.
	void packetRxd(int signature, QDataStream & ds);

	void sendPacket(int signature, const QByteArray data);	///< intenally used to trigger sending in the connected CommSocket(s)
public slots:
	void sendSlot(int signature, const QByteArray data);
protected slots:
	void disconnectedSlot();		///< internal to catch disconnects
protected:
	/// called when a new connection comes in
	/// will create a CommSocket set up signals/slots
	virtual void incomingConnection(quintptr socketDescriptor);
	QString m_name;				///< the name used for this server
	QList<CommSocket*> sockets() const;	///< get all connected sockets
	QByteArray m_txData;	///< collects data to tx
	QBuffer m_txBuffer;	///< interface to m_txData for m_txStream
	QDataStream m_txStream;	///< the interface for the user
	void resetBuffer();	///< resets the tx buffer after sending
};

#endif
