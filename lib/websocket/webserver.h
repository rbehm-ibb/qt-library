// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/20/2017 by behm
// ******************************************************

#ifndef WEBSERVER_H
#define WEBSERVER_H

class WebResource;

class WebServer : public QObject
{
	Q_OBJECT
public:
	explicit WebServer(quint16 port, QObject *parent = 0);
	~WebServer();
	void addResource(WebResource *rsc);
	void close();
signals:

public slots:
private slots:
	void acceptError(QAbstractSocket::SocketError socketError);
	void newConnection();
	void originAuthenticationRequired(QWebSocketCorsAuthenticator * authenticator);
//	void peerVerifyError(const QSslError & error);
	void serverError(QWebSocketProtocol::CloseCode closeCode);
protected:
private:
	QWebSocketServer *m_server;

	// QObject interface
protected:
	virtual void childEvent(QChildEvent *event);
};

#endif // WEBSERVER_H
