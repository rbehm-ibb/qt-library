// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef COMMSOCKET_H
#define COMMSOCKET_H

/// \brief Socket for communication between processes, Works together with CommServer.
///
/// It uses QLocalSocket inside and allows transfer of data blocks.
/// It handles the reception, wait for all data and finally signals reception via signals.
/// It mimics QCopChannel from Qt/E
///
/// Its counterpart is CommServer.
///
/// \par Data is sent in the form
/// \li int signature identifies the meaning of the data, mostly it is just CommSocket::Normaldata, but can be extended
/// \li int byte count of the following real data
/// \li data just uninterpreted bytes
///
/// \par It has two interfaces for the application:
/// -# Based on QByteArray. Data to send is just given in a QByteArray. This is only used by CommServer
///    - Received data is delivered via dataRxd() inside a QByteArray
/// -# Based on QDataStream. Data is collected using << operators for QDataStream and finally sent using sendPacket():
/// This also resets the stream.
///    - Received data is also delivered using packetRxd() in a QDataStream
///
/// The QDataStream is easier to use and also mirrored in CommServer.
///

class CommSocket : public QLocalSocket
{
	Q_OBJECT
	friend class CommServer;
public:
	typedef quint16 CommSig;
	enum CommId	/// these are the default ids, others may define more
	{
		NormalData = 0,	///< used for most data when there is only one type
		TimeoutData = -1	///< signals timeout from external source
	};
	/// \brief creates a normal CommSocket with name and parent
	/// \param name used to connect to the server of the same name
	/// \param parent the usual parent for a QObject
	CommSocket(QString name, QObject *parent = 0);
	~CommSocket();

	bool isConnected() const { return state() == QLocalSocket::ConnectedState; }	///< are we connected to the server?
	QDataStream & txStream() { return m_txStream; }		///< an internally created QDataStream so that the << operators can be easily used.
	void sendPacket(int signature = NormalData);	///< sends data with signature after it has been <<'ed into the txStream(). Resets the stream.
	void setTimeout(int time);	///< start time out watchdog. It fires (via watchDogTimeout() ) when nothing has been received.
signals:
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
	void watchDogReset();
	void watchDogTimeout();					///< timeout, nothing received
	void stateChanged(bool connected);			///< the connection state has changed
private slots:
	void sendDirect(int signature, const QByteArray data);	///< send data with out using stream

//	void catchWdReset();
//	void catchPacket(int signature, const QByteArray data);
//	void catchWdTimeout();
protected slots:
	void connectToServer();					///< tries to (re)connect to the server
	void readyReadSlot();					///< something has been received
	void connectedSlot();					///< we are connected
	void disconnectedSlot();				///< we are disconnected

protected:
	QString m_name;		///< the name given in constructor
	enum State	/// for internal state machine
	{
		WaitForSignature,	///< idle, we are waiting for the signature
		WaitForSize,		///< got sig, wait for size
		WaitForData		///< get size wait for data
	};
	State m_rxState;	///< for internal state machine
	int m_sig;		///< received signature
	qint32 m_size;		///< received size
	QByteArray m_rxdData;	///< to collect received data
	QTimer * m_reconnectTimer;	///< used for reconnection timing
	QTimer * m_watchDogTimer;	///< used to watch for any reception and signal timeout
	static const QString m_prefix;	///< prepended to name to give a filesystem location for QLocalSocket
	QByteArray m_txData;	///< used internally for buffer for QDataStream
	QBuffer m_txBuffer;	///< the buffer for handling QDataStream
	QDataStream m_txStream;	///< a QDataStream to easiyl use << operators for sending
	void resetBuffer();	///< resets the txStream
private:
	/// \brief creates a CommSocket using socketDescriptor
	/// This is made private, since it is only for use by CommServer to create its internal socket
	/// \param socketDescriptor
	/// \param name (of the server)
	/// \param parent the usual parent for a QObject
	CommSocket(quintptr socketDescriptor, QString name, QObject *parent = 0);
	/// only for CommServer to have the prefix only at one place.
	/// has been put here since CommServer uses CommSocket internally
	static const QString prefix() { return m_prefix; }
};

#endif
