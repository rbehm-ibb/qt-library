// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

// #include "project.h"
#include "tcpcommsocket.h"

TcpCommSocket::TcpCommSocket(QString ip, quint16 port, QObject *parent)
	: QTcpSocket(parent)
	, m_ip(ip)
	, m_port(port)
	, m_rxState(WAIT_SIG)
	, m_size(0)
	, m_reconnectTimer(new QTimer(this))
{
	qDebug() << Q_FUNC_INFO << ip << port;
	connect(this, &QTcpSocket::connected, this, &TcpCommSocket::connectedSlot);
	connect(this, &QTcpSocket::disconnected, this, &TcpCommSocket::disconnectedSlot);
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSlot(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChangedSlot(QAbstractSocket::SocketState)));
	connect(this, &QTcpSocket::readyRead, this, &TcpCommSocket::readyReadSlot);
	connect(m_reconnectTimer, &QTimer::timeout, this, &TcpCommSocket::connectToServer);
//	connect(m_watchDogTimer, &QTimer::timeout, this, &TcpCommSocket::watchDogTimeout);
	m_reconnectTimer->start(1000);
}

TcpCommSocket::TcpCommSocket(quintptr socketDescriptor, QObject *parent)
	: QTcpSocket(parent)
	, m_rxState(WAIT_SIG)
	, m_size(0)
	, m_reconnectTimer(0)
{
//	qDebug() << Q_FUNC_INFO << socketDescriptor;
	setSocketDescriptor(socketDescriptor);
	connect(this, &QTcpSocket::connected, this, &TcpCommSocket::connectedSlot);
	connect(this, &QTcpSocket::disconnected, this, &TcpCommSocket::disconnectedSlot);
//	connect(this, SIGNAL(error(QTcpSocket::LocalSocketError)), SLOT(errorSlot(QTcpSocket::LocalSocketError)));
	connect(this, &QTcpSocket::stateChanged, this, &TcpCommSocket::stateChangedSlot);
	connect(this, &QTcpSocket::readyRead, this, &TcpCommSocket::readyReadSlot);
}

TcpCommSocket::~TcpCommSocket()
{
// 	qDebug() << Q_FUNC_INFO << name;
}

void TcpCommSocket::connectToServer()
{
	if (state() != QAbstractSocket::ConnectedState)
	{
		QTcpSocket::connectToHost(m_ip, m_port);
	}
}

void TcpCommSocket::send(quint16 signature, const QByteArray data)
{
//	qDebug() << Q_FUNC_INFO  << signature << data.size();
	if (isValid())
	{
		write((const char*)&signature, sizeof(signature));
		quint32 size = data.size();
		write((const char*)&size, sizeof(size));
		write(data);
	}
	else
	{
		qWarning()  << Q_FUNC_INFO << m_ip << "not connected";
	}
}

void TcpCommSocket::readyReadSlot()
{
// 	qDebug() << Q_FUNC_INFO << name << "State=" << state << "Size=" << size << "Read=" << rxdData.size();
	while(bytesAvailable() > 0)
	{
		switch(m_rxState)
		{
		case WAIT_SIG:
			if (size_t(bytesAvailable()) >= sizeof(m_sig))
			{
				read((char*)&m_sig, sizeof(m_sig));
				m_rxState = WAIT_SIZE;
				m_rxdData.clear();
			}
			break;
		case WAIT_SIZE:
			if (size_t(bytesAvailable()) >= sizeof(m_size))
			{
				read((char*)&m_size, sizeof(m_size));
//				qDebug() << Q_FUNC_INFO << "Sig=" << m_sig << "Size=" << m_size;
				if (m_size == 0)	// empty data
				{
					m_rxState = WAIT_SIG;
//					qDebug() << Q_FUNC_INFO << name << "Sig=" << sig << "Size=0";
					emit dataRxd(m_sig, QByteArray());
				}
				else
				{
					m_rxState = WAIT_DATA;
					m_rxdData.reserve(m_size);
				}
			}
			break;
		case WAIT_DATA:
			QByteArray rd = read(m_size - m_rxdData.size());	// try to read as much as expected
			m_rxdData.append(rd);
//			qDebug() << "read " << rd.size() << m_rxdData.size();
			if (m_rxdData.size() >= m_size)
			{
				m_rxState = WAIT_SIG;
//				m_watchDogTimer->start();
				emit dataRxd(m_sig, m_rxdData);
//				qDebug() << Q_FUNC_INFO << "Sig=" << int(m_sig) << "Size=" << m_size << m_rxdData.toHex();
			}
			break;
		}
	}
}

void TcpCommSocket::connectedSlot()
{
//	qDebug() << Q_FUNC_INFO << m_ip <<  m_port;
	if (m_reconnectTimer)
	{
		m_reconnectTimer->stop();
	}
	m_rxState = WAIT_SIG;
}

void TcpCommSocket::disconnectedSlot()
{
	if (m_reconnectTimer)
	{
//		qDebug() << Q_FUNC_INFO << m_reconnectTimer << m_ip <<  m_port;
		m_reconnectTimer->start(1000);
	}
}

void TcpCommSocket::errorSlot(QAbstractSocket::SocketError socketError)
{
	Q_UNUSED(socketError)
//	qDebug() << Q_FUNC_INFO  << socketError << m_ip <<  m_port;
}

void TcpCommSocket::stateChangedSlot(QAbstractSocket::SocketState socketState)
{
	Q_UNUSED(socketState)
//	qDebug() << Q_FUNC_INFO << socketState << m_ip <<  m_port;
}
