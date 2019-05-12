// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/29/2018 by behm
// ******************************************************

#include "singleapp.h"

SingleApp::SingleApp(bool kill, QObject *parent)
	: QObject(parent)
{
	const QString dirName("singleApp");
//	QDir dir("/var/lock");
	QDir dir("/tmp/");
	if (! dir.exists(dirName))
	{
		qDebug() << Q_FUNC_INFO << "Create" << dir.filePath(dirName);
		if (! dir.mkdir(dirName))
		{
			qWarning() << Q_FUNC_INFO << "problem creating" << dir.filePath(dirName);
		}
	}
	dir.cd(dirName);
	m_sockName = dir.absoluteFilePath(qApp->applicationName());
//	qDebug() << Q_FUNC_INFO << m_sockName;
	m_socket = new QLocalSocket(this);
	m_socket->connectToServer(m_sockName);
	if (m_socket->waitForConnected(1000))
	{
		connect(m_socket.data(), &QLocalSocket::disconnected, this, &SingleApp::disconnectedSlot);
		qDebug() << Q_FUNC_INFO << "found other instance" << m_sockName;
		if (kill)
		{
			m_socket->write("kill\n");
			m_socket->waitForBytesWritten();
//			m_socket->close();
		}
		else
		{
			QMessageBox::critical(nullptr, qApp->applicationName(), "Another instance is running.");
			exit(1);
		}
	}
	else
	{
//		qDebug() << Q_FUNC_INFO << m_socket->error();
		delete m_socket;
	}
	disconnectedSlot();
}

SingleApp::~SingleApp()
{
//	qDebug() << Q_FUNC_INFO << m_socket << m_server;
	if (m_server)
		QFile::remove(m_sockName);
}

void SingleApp::newConnection()
{
//	qDebug() << Q_FUNC_INFO;
	m_socket = m_server->nextPendingConnection();
//	connect(m_socket, &QLocalSocket::connected, this, &SingleApp::connectedSlot);
	connect(m_socket.data(), &QLocalSocket::disconnected, this, &SingleApp::disconnectedSlot);
	connect(m_socket.data(), &QLocalSocket::readyRead, this, &SingleApp::readyReadSlot);
}

void SingleApp::readyReadSlot()
{
//	qDebug() << Q_FUNC_INFO << m_socket;
	while (m_socket->canReadLine())
	{
		QByteArray l = m_socket->readLine();
		if (l == "kill\n")
		{
			qDebug() << Q_FUNC_INFO << "SingleApp: killed";
			emit kill();
			m_socket->close();
			if (m_server)
			{
				m_server->close();
				delete m_server;
				QFile::remove(m_sockName);
			}
			QTimer::singleShot(1000, qApp, SLOT(quit()));
//			qApp->quit();
			return;
		}
	}
}

void SingleApp::disconnectedSlot()
{
	if (! m_server)
	{
		m_server = new QLocalServer(this);
		m_server->listen(m_sockName);
		connect(m_server.data(), &QLocalServer::newConnection, this, &SingleApp::newConnection);
//		qDebug() << Q_FUNC_INFO << m_server->fullServerName() << m_server->isListening();
		return;
	}
//	qDebug() << Q_FUNC_INFO << m_socket << m_server;
	m_socket->deleteLater();
	m_socket = 0;
}
