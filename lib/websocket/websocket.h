// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/20/2017 by behm
// ******************************************************

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

class WebResource;

class WebSocket : public QObject
{
	Q_OBJECT
public:
	explicit WebSocket(WebResource *rsc, QWebSocket *socket, QObject *parent = 0);
	explicit WebSocket(WebResource *rsc, const QString origin, const QString host, quint16 port, QObject *parent = 0);
	~WebSocket();

	WebResource* rsc() const;
	void disconnect();
	void autoConnect();

signals:
	void stateChanged(QAbstractSocket::SocketState state);
	void connectedSig();
	void disconnectedSig(const QString msg);
public slots:
	void connectToHost();
	void close();
	void sendData(qint16 type, const QByteArray &data, WebSocket *dst);
	virtual void update();
private slots:
	void disconnectedSlot();
	void connectedSlot();
	void binaryMessageReceived(const QByteArray & message);
protected:
private:
	bool socketCreated;
	QPointer<WebResource> m_rsc;
	QPointer<QWebSocket> m_socket;
	QString m_origin;
	QString m_host;
	quint16 m_port;
	QCryptographicHash m_hash;
	QTimer *m_connectTimer;
};

#endif // WEBSOCKET_H
