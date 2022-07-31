// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/10/2022 by behm
// ******************************************************

#ifndef ZCONFSERVER_H
#define ZCONFSERVER_H

#include "zconftable.h"

class IpcTcpServer;
class ModelSynchOut;

class ZConfServer : public QObject
{
	Q_OBJECT
public:
	explicit ZConfServer(QObject *parent = nullptr);
	void open(uint16_t port);
	void setTcpPort(uint16_t port);
	void setMaxAge(int age);
	ZConfTable *table() const { return m_table; }
signals:
	void rxd(QString msg);
private slots:
	void readyReadRx();
	void stateChangedRx(QAbstractSocket::SocketState socketState);
private:
	QPointer<QUdpSocket> m_rx;
	QPointer<IpcTcpServer> m_tcp;
	ZConfTable *m_table;
	ModelSynchOut *m_synch;
};

#endif // ZCONFSERVER_H
