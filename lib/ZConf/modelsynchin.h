// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/13/2022 by behm
// ******************************************************

#ifndef MODELSYNCHIN_H
#define MODELSYNCHIN_H

class IpcTcpSocket;
class ZConfTable;

class ModelSynchIn : public QObject
{
	Q_OBJECT
public:
	enum SynchCmd { Reset, Insert, Remove, Update };	// must be in snch with ..Out
	Q_ENUM(SynchCmd)
	explicit ModelSynchIn(QObject *parent = nullptr);
	void setModel(ZConfTable *model);
	void setSocket(IpcTcpSocket *socket);

signals:
	void updated();
private slots:
	void dataRxd(int signature, const QByteArray data);
private:
	ZConfTable *m_model;
	IpcTcpSocket *m_socket;
};

#endif // MODELSYNCHIN_H
