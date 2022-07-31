// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 9/7/2018 by behm
// ******************************************************

#include "ipctcpsocket.h"

IpcTcpSocket::IpcTcpSocket(QObject *parent)
	: QTcpSocket(parent)
	, m_watchDogTimer(new QTimer(this))
	, m_rxState(WaitForSignature)
	, m_size(0)
{
//	qDebug() << Q_FUNC_INFO;
	connect(this, &QTcpSocket::connected, this, &IpcTcpSocket::connectedSlot);
	connect(this, &QTcpSocket::disconnected, this, &IpcTcpSocket::disconnectedSlot);
	connect(this, &QTcpSocket::readyRead, this, &IpcTcpSocket::readyReadSlot);
	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
	connect(m_watchDogTimer, &QTimer::timeout, this, &IpcTcpSocket::timeout);

}

IpcTcpSocket::IpcTcpSocket(quintptr socketDescriptor, QObject *parent)
	: QTcpSocket(parent)
	, m_watchDogTimer(new QTimer(this))
	, m_rxState(WaitForSignature)
	, m_size(0)
{
//	qDebug() << Q_FUNC_INFO;
	setSocketDescriptor(socketDescriptor);
	connect(this, &QTcpSocket::connected, this, &IpcTcpSocket::connectedSlot);
	connect(this, &QTcpSocket::disconnected, this, &IpcTcpSocket::disconnectedSlot);
	connect(this, &QTcpSocket::readyRead, this, &IpcTcpSocket::readyReadSlot);
	connect(m_watchDogTimer, &QTimer::timeout, this, &IpcTcpSocket::timeout);

}

IpcTcpSocket::~IpcTcpSocket()
{
	disconnectFromHost();
	close();
}

void IpcTcpSocket::send(quint8 signature, const QByteArray data)
{
	if (isConnected())
	{
//		qDebug()  << Q_FUNC_INFO << m_name << signature << data.size();
		write((const char*)&signature, sizeof(signature));
		quint16 size = data.size();
		write((const char*)&size, sizeof(size));
		write(data.constData(), size);
	}
	else
	{
		qWarning()  << Q_FUNC_INFO << "not connected" << state() << signature << data.size();
	}

}

void IpcTcpSocket::connectToHost(QHostAddress host, quint16 port)
{
	if (! isConnected())
	{
//		qDebug() << Q_FUNC_INFO << host << port << isConnected();
		QTcpSocket::connectToHost(host, port);
	}
}

void IpcTcpSocket::readyReadSlot()
{
	while(bytesAvailable() > 0)
	{
		switch(m_rxState)
		{
		case WaitForSignature:
			if (size_t(bytesAvailable()) >= sizeof(m_sig))
			{
				read((char*)&m_sig, sizeof(m_sig));
				m_rxState = WaitForSize;
				m_rxdData.clear();
			}
			break;
		case WaitForSize:
			if (size_t(bytesAvailable()) >= sizeof(m_size))
			{
				read((char*)&m_size, sizeof(m_size));
//				qDebug() << "read size" << m_size;
				if (m_size == 0)	// empty data
				{
					m_rxState = WaitForSignature;
//					qDebug() << Q_FUNC_INFO << m_name << "Sig=" << m_sig << "Size=0";
//					emit watchDogReset();
					emit dataRxd(m_sig, QByteArray());
//					QDataStream ds(m_rxdData);
//					emit packetRxd(m_sig, ds);
				}
				else
				{
					m_rxState = WaitForData;
					m_rxdData.reserve(m_size);
				}
			}
			break;
		case WaitForData:
			QByteArray rd = read(m_size - m_rxdData.size());	// try to read as much as expected
			m_rxdData.append(rd);
//			qDebug() << "read " << rd.size() << m_rxdData.size();
			if (m_rxdData.size() >= m_size)
			{
				m_rxState = WaitForSignature;
//				qDebug() << Q_FUNC_INFO << m_name << "Sig=" << m_sig << "Size=" << m_size << m_rxdData.size();
//				emit watchDogReset();
				emit dataRxd(m_sig, m_rxdData);
//				QDataStream ds(m_rxdData);
//				emit packetRxd(m_sig, ds);
			}
			break;
		}
	}

}

void IpcTcpSocket::connectedSlot()
{
	m_rxState = WaitForSignature;
	emit connectStateChanged(true);
}

void IpcTcpSocket::disconnectedSlot()
{
	emit connectStateChanged(false);
}

void IpcTcpSocket::timeout()
{
	qWarning()  << Q_FUNC_INFO << m_rxState<< m_rxdData.size() << state() << peerName() << peerPort();
	m_rxState = WaitForSignature;
	m_rxdData.clear();
}

void IpcTcpSocket::stateChanged(QAbstractSocket::SocketState socketState)
{
//	qDebug() << Q_FUNC_INFO << socketState;
}

