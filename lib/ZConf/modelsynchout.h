// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/13/2022 by behm
// ******************************************************

#ifndef MODELSYNCHOUT_H
#define MODELSYNCHOUT_H

class ZConfTable;
class IpcTcpServer;
class IpcTcpSocket;

class ModelSynchOut : public QObject
{
	Q_OBJECT
public:
	enum SynchCmd { Reset, Insert, Remove, Update };
	Q_ENUM(SynchCmd)
	explicit ModelSynchOut(IpcTcpServer *parent = nullptr);
	void setModel(ZConfTable *model);

signals:
	void send(quint8 signature, const QByteArray data);	///< send data with out using stream
private slots:
	void dataRxd(int signature, const QByteArray data);
	void fullUpdate(IpcTcpSocket *socket);
	// IF to model
	void modelReset();
	void rowsInserted(const QModelIndex &parent, int first, int last);
	void rowsRemoved(const QModelIndex &parent, int first, int last);
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

private:
	ZConfTable *m_model;
	IpcTcpServer *m_server;
};

#endif // MODELSYNCH_H
