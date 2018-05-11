// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/20/2017 by behm
// ******************************************************

#include "websocket.h"
#include "webresource.h"
// #include "rcsids.h"

WebSocket::WebSocket(WebResource *rsc, QWebSocket *socket, QObject *parent)
	: QObject(parent)
	, socketCreated(false)
	, m_rsc(rsc)
	, m_socket(socket)
	, m_hash(QCryptographicHash::Md5)
	, m_connectTimer(nullptr)
{
	setObjectName(rsc->objectName());
	m_socket->setObjectName(rsc->objectName());
	socket->setParent(this);	// take responsibility
	connect(socket, &QWebSocket::disconnected, this, &WebSocket::disconnectedSlot);
	connect(socket, &QWebSocket::binaryMessageReceived, this, &WebSocket::binaryMessageReceived);
	connect(socket, &QWebSocket::stateChanged, this, &WebSocket::stateChanged);
	connect(rsc, &WebResource::sendData, this, &WebSocket::sendData);
}

WebSocket::WebSocket(WebResource *rsc, const QString origin, const QString host, quint16 port, QObject *parent)
	: QObject(parent)
	, socketCreated(true)
	, m_rsc(rsc)
	, m_origin(origin)
	, m_host(host)
	, m_port(port)
	, m_hash(QCryptographicHash::Md5)
	, m_connectTimer(0)
{
	setObjectName(rsc->objectName());
//	qDebug() << Q_FUNC_INFO << objectName();
}

WebSocket::~WebSocket()
{
//	qWarning() << Q_FUNC_INFO << objectName();
}

WebResource *WebSocket::rsc() const
{
	return m_rsc.data();
}

void WebSocket::connectToHost()
{
	if (! socketCreated)
	{
		qWarning() << Q_FUNC_INFO << "foreign socket";
		return;
	}
	if (m_connectTimer)
	{
		m_connectTimer->stop();	// no reconnect
	}
	if (m_socket)
	{
		m_socket->deleteLater();
	}
	QUrl urlo;
	urlo.setScheme("ws");
	urlo.setHost(m_origin);
	urlo.setPath("/" + m_rsc->objectName());
	QUrl url;
	url.setScheme("ws");
	url.setHost(m_host);
	url.setPort(m_port);
	url.setPath("/" + m_rsc->objectName());
//	qDebug() << Q_FUNC_INFO << url << urlo;
	m_socket = new QWebSocket(urlo.toString(), QWebSocketProtocol::VersionLatest, this);
	m_socket->setObjectName(m_rsc->objectName());

	connect(m_socket.data(), &QWebSocket::disconnected, this, &WebSocket::disconnectedSlot);
	connect(m_socket.data(), &QWebSocket::connected, this, &WebSocket::connectedSlot);
	connect(m_socket.data(), &QWebSocket::binaryMessageReceived, this, &WebSocket::binaryMessageReceived);
	connect(m_socket.data(), &QWebSocket::stateChanged, this, &WebSocket::stateChanged);
	connect(m_rsc.data(), &WebResource::sendData, this, &WebSocket::sendData);
	m_socket->open(url.toString());
}

void WebSocket::close()
{
	if (! m_connectTimer)
	{
		m_connectTimer->stop();	// no reconnect
		m_connectTimer->deleteLater();
		m_connectTimer = nullptr;
	}
	if (m_socket)
	{
		m_socket->close(QWebSocketProtocol::CloseCodePolicyViolated, "NoAuth");
	}

}

void WebSocket::disconnect()
{
	if (m_socket)
	{
		m_socket->close();
		if (m_connectTimer)
		{
			m_connectTimer->stop();	// no reconnect
		}
	}
}

void WebSocket::autoConnect()
{
	if (! m_connectTimer)
	{
		m_connectTimer = new QTimer(this);
		connect(m_connectTimer, &QTimer::timeout, this, &WebSocket::connectToHost);
		m_connectTimer->start(5*1000);
	}
}

void WebSocket::disconnectedSlot()
{
//	qDebug() << Q_FUNC_INFO << objectName();
	emit disconnectedSig(m_socket->errorString());
	if (m_socket)
	{
//		qDebug() << Q_FUNC_INFO << m_socket->closeCode() << m_socket->closeReason() << m_socket->errorString();
		m_socket->deleteLater();
	}
	if (! socketCreated)
	{
		deleteLater();
	}
	else
	{
		if (m_connectTimer)
		{
			m_connectTimer->start(5*1000);
		}
	}
}

void WebSocket::connectedSlot()
{
//	qDebug() << Q_FUNC_INFO << objectName() << m_socket->peerName();
	if (m_connectTimer)
	{
		m_connectTimer->stop();	// no reconnect
	}
	emit connectedSig();
}

void WebSocket::binaryMessageReceived(const QByteArray &message)
{
	if (! m_rsc)
	{
		qWarning() << Q_FUNC_INFO << "No Resource" << objectName();
		return;
	}
	QDataStream ds(message);
	quint16 type;
	QByteArray data;
	QByteArray hash;
	ds >> type >> data >> hash;
//	qDebug() << Q_FUNC_INFO << ResIdGadget::ResId(type);
	if (ds.status() != QDataStream::Ok)
	{
		qWarning() << Q_FUNC_INFO << "read error" << ds.status();
		return;
	}
	m_hash.reset();
	m_hash.addData(data);
	if (m_hash.result() != hash)
	{
		qWarning() << Q_FUNC_INFO << "hash error" << m_hash.result().toHex() << hash.toHex();
		return;
	}
	m_rsc.data()->setRxdData(type, data, this);
}

void WebSocket::sendData(qint16 type, const QByteArray &data, WebSocket *dst)
{
	if (dst && dst != this)
		return;
//	qDebug() << Q_FUNC_INFO << objectName() << ResIdGadget::ResId(type);
	if (! m_socket || ! m_socket.data()->isValid())
	{
		qWarning() << Q_FUNC_INFO << "No Socket" << objectName();
		return;
	}
	if (! m_rsc)
	{
		qWarning() << Q_FUNC_INFO << "No Resource" << objectName();
		return;
	}
//	const QByteArray data = m_rsc.data()->dataToSend();
	m_hash.reset();
	m_hash.addData(data);
	const QByteArray hash = m_hash.result();
	QByteArray dout;
	{
		QDataStream ds(&dout, QIODevice::WriteOnly);
		ds << type << data << hash;
	}
//	qDebug() << Q_FUNC_INFO << dout.toHex() << data.toHex() << hash.toHex();
	m_socket.data()->sendBinaryMessage(dout);
}

void WebSocket::update()
{
	sendData(WebResource::FullUpdate, m_rsc.data()->dataToSend(), this);
}
