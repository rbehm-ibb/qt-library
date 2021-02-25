// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/20/2017 by behm
// ******************************************************

#include "webserver.h"
#include "websocket.h"
#include "webresource.h"

WebServer::WebServer(quint16 port, QObject *parent)
	: QObject(parent)
	, m_server(new QWebSocketServer("IBB/WebServer", QWebSocketServer::NonSecureMode, this))
{
	if (! m_server->listen(QHostAddress::Any, port))
	{
		qWarning() << Q_FUNC_INFO << "listen" << m_server->errorString();
	}
	else
	{
		qDebug() << Q_FUNC_INFO << "listining on port" << port;
		connect(m_server, &QWebSocketServer::newConnection, this, &WebServer::newConnection);
		connect(m_server, &QWebSocketServer::acceptError, this, &WebServer::acceptError);
		connect(m_server, &QWebSocketServer::serverError, this, &WebServer::serverError);
		connect(m_server, &QWebSocketServer::originAuthenticationRequired, this, &WebServer::originAuthenticationRequired);
	}

}

WebServer::~WebServer()
{
	qDebug() << Q_FUNC_INFO << this;
}

void WebServer::addResource(WebResource *rsc)
{
	rsc->setParent(this);
}

void WebServer::close()
{
	qDebug() << Q_FUNC_INFO << this;
	if (m_server)
	{
		m_server->close();
	}
}

void WebServer::newConnection()
{
	QWebSocket *socket = m_server->nextPendingConnection();
	qDebug() << Q_FUNC_INFO << socket;
	if (socket)
	{
		qDebug() << socket->peerName() << socket->peerPort() << socket->origin() << socket->resourceName();
		WebResource *rsc = findChild<WebResource*>(QUrl(socket->resourceName()).fileName());
		if (rsc)
		{
			WebSocket *ws = new WebSocket(rsc, socket, this);
			ws->update();	// force sending
		}
		else
		{
			qWarning() << Q_FUNC_INFO << "Unknown reseource" << QUrl(socket->resourceName()).fileName();
			delete socket;
		}
	}

}

void WebServer::acceptError(QAbstractSocket::SocketError socketError)
{
	qWarning() << Q_FUNC_INFO << socketError << m_server->errorString();
}

void WebServer::originAuthenticationRequired(QWebSocketCorsAuthenticator *authenticator)
{
	QUrl url(authenticator->origin());
//	qDebug() << Q_FUNC_INFO << url;
	WebResource *rsc = findChild<WebResource*>(url.fileName());
	if (! rsc)
	{
		qDebug() << Q_FUNC_INFO << "no Resource" << rsc;
		authenticator->setAllowed(false);
	}
}

void WebServer::serverError(QWebSocketProtocol::CloseCode closeCode)
{
	qDebug() << Q_FUNC_INFO << closeCode;
}

void WebServer::childEvent(QChildEvent *event)
{
	if (event->added())
	{
//		qDebug() << Q_FUNC_INFO << "added" << event;
		WebResource *rsc = qobject_cast<WebResource*>(event->child());
//		qDebug() << Q_FUNC_INFO << "added1" << rsc;
		if (rsc)
		{
			WebResource *rsc1 = findChild<WebResource*>(rsc->objectName());
			if (rsc1 && rsc1 != rsc)
			{
				qWarning() << Q_FUNC_INFO << "not unique" << rsc->objectName();
			}
		}
	}
	if (event->polished())
	{
		qDebug() << Q_FUNC_INFO << "polished" << event;
	}
}
