// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/10/2022 by behm
// ******************************************************

#ifndef ZCONFCLIENT_H
#define ZCONFCLIENT_H

#include "zconftable.h"
class IpcTcpSocket;
class ModelSynchIn;

class ZconfClient : public QObject
{
	Q_OBJECT
public:
	explicit ZconfClient(QObject *parent = nullptr);
	void open(uint16_t port, const QString id = QString());
	ZConfTable *table() const { return m_table; }
	QString hostInfo() const;

signals:
	void updated();
	void rxd(const QString msg);
	void foundServer(QHostAddress ip, uint16_t port);
	void connectState(bool on);
private slots:
	void stateChangedUdp(QAbstractSocket::SocketState socketState);
	void readyReadUdp();
	void stateChangedTcp(QAbstractSocket::SocketState socketState);
	void errorOccurred(QAbstractSocket::SocketError socketError);
//	void dataRxdIpc(int signature, const QByteArray data);
private:
	QPointer<QUdpSocket> m_udp;
	QPointer<IpcTcpSocket> m_tcp;
	ModelSynchIn *m_synch;
	ZConfTable *m_table;
};

#endif // ZCONFCLIENT_H
