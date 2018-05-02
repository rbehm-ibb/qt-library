// ******************************************************
// * copyright (C) by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#include "project.h"
#include "commsocket.h"

const QString CommSocket::m_prefix("BETL.AvioScout-");

CommSocket::CommSocket(QString name, QObject *parent)
	: QLocalSocket(parent)
	, m_name(name)
	, m_rxState(WaitForSignature)
	, m_size(0)
	, m_reconnectTimer(new QTimer(this))
	, m_watchDogTimer(0)
{
//	qDebug() << Q_FUNC_INFO << m_name;
	connect(this, &QLocalSocket::connected, this, &CommSocket::connectedSlot);
	connect(this, &QLocalSocket::disconnected, this, &CommSocket::disconnectedSlot);
	connect(this, &QLocalSocket::readyRead, this, &CommSocket::readyReadSlot);
	connect(m_reconnectTimer, &QTimer::timeout, this, &CommSocket::connectToServer);
	m_reconnectTimer->setInterval(1000);
	m_reconnectTimer->start();
	resetBuffer();
}

CommSocket::CommSocket(quintptr socketDescriptor, QString name, QObject *parent)
	: QLocalSocket(parent)
	, m_name(name)
	, m_rxState(WaitForSignature)
	, m_size(0)
	, m_reconnectTimer(0)
	, m_watchDogTimer(0)
{
//	qDebug() << Q_FUNC_INFO << socketDescriptor;
	setSocketDescriptor(socketDescriptor);
	connect(this, &QLocalSocket::connected, this, &CommSocket::connectedSlot);
	connect(this, &QLocalSocket::disconnected, this, &CommSocket::disconnectedSlot);
	connect(this, &QLocalSocket::readyRead, this, &CommSocket::readyReadSlot);
	resetBuffer();
}

CommSocket::~CommSocket()
{
//	qDebug() << Q_FUNC_INFO << m_name;
}

void CommSocket::connectToServer()
{
	const QString name = CommSocket::prefix() + m_name;
//	qDebug() << Q_FUNC_INFO << name;
	QLocalSocket::connectToServer(name);
}

void CommSocket::sendDirect(int signature, const QByteArray data)
{
	if (isConnected())
	{
//		qDebug()  << Q_FUNC_INFO << m_name << signature << data.size();
		write((const char*)&signature, sizeof(signature));
		quint32 size = data.size();
		write((const char*)&size, sizeof(size));
		write(data.constData(), size);
	}
	else
	{
		qWarning()  << Q_FUNC_INFO << m_name << "not connected" << state() << signature << data.size();
	}
}

void CommSocket::sendPacket(int signature)
{
	sendDirect(signature, m_txData);
	resetBuffer();
}

void CommSocket::setTimeout(int time)
{
	if (! m_watchDogTimer)
	{
		m_watchDogTimer = new QTimer(this);
		connect(m_watchDogTimer, &QTimer::timeout, this, &CommSocket::watchDogTimeout);
		connect(this, SIGNAL(watchDogReset()), m_watchDogTimer, SLOT(start()));
//		if (m_name != "mcu" && m_name != "gps")
//		{
//			connect(m_watchDogTimer, SIGNAL(timeout()), this, SLOT(catchWdTimeout()));
//			connect(this, SIGNAL(watchDogReset()), this, SLOT(catchWdReset()));
//			connect(this, SIGNAL(dataRxd(int,QByteArray)), this, SLOT(catchPacket(int,QByteArray)));
//		}
	}
	m_watchDogTimer->setInterval(time);
	m_watchDogTimer->start();
//	qDebug() << Q_FUNC_INFO << m_name << time;
}

void CommSocket::readyReadSlot()
{
//	qDebug() << Q_FUNC_INFO << m_name << "State=" << m_rxState << "Size=" << m_size << "Read=" << m_rxdData.size();
	while(bytesAvailable() > 0)
	{
		switch(m_rxState)
		{
		case WaitForSignature:
			if (bytesAvailable() >= sizeof(m_sig))
			{
				read((char*)&m_sig, sizeof(m_sig));
				m_rxState = WaitForSize;
				m_rxdData.clear();
			}
			break;
		case WaitForSize:
			if (bytesAvailable() >= sizeof(m_size))
			{
				read((char*)&m_size, sizeof(m_size));
//				qDebug() << "read size" << m_size;
				if (m_size == 0)	// empty data
				{
					m_rxState = WaitForSignature;
//					qDebug() << Q_FUNC_INFO << m_name << "Sig=" << m_sig << "Size=0";
					emit watchDogReset();
					emit dataRxd(m_sig, QByteArray());
					QDataStream ds(m_rxdData);
					emit packetRxd(m_sig, ds);
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
				emit watchDogReset();
				emit dataRxd(m_sig, m_rxdData);
				QDataStream ds(m_rxdData);
				emit packetRxd(m_sig, ds);
			}
			break;
		}
	}
}

void CommSocket::connectedSlot()
{
//	qDebug() << Q_FUNC_INFO << m_name;
	if (m_reconnectTimer)
	{
		m_reconnectTimer->stop();
	}
	m_rxState = WaitForSignature;
	resetBuffer();
	emit stateChanged(true);
}

void CommSocket::disconnectedSlot()
{
//	qDebug() << Q_FUNC_INFO << m_name;
	if (m_reconnectTimer)
	{
		m_reconnectTimer->start();
	}
	emit stateChanged(false);
}

void CommSocket::resetBuffer()
{
	m_txBuffer.close();
	m_txStream.setDevice(0);
	m_txData.clear();
	m_txBuffer.setBuffer(&m_txData);
	m_txBuffer.open(QIODevice::WriteOnly);
	m_txStream.setDevice(&m_txBuffer);
}

#if 0
static QElapsedTimer etim;

void CommSocket::catchWdReset()
{
	qDebug() << Q_FUNC_INFO << m_name << m_watchDogTimer->remainingTime() << etim.elapsed();
}

void CommSocket::catchPacket(int signature, const QByteArray data)
{
	qDebug() << Q_FUNC_INFO << m_name << signature << data.size() << m_watchDogTimer->remainingTime() << etim.elapsed();
	etim.restart();
}

void CommSocket::catchWdTimeout()
{
	qDebug() << Q_FUNC_INFO << m_name << etim.elapsed() << m_watchDogTimer->interval();
}
#endif
