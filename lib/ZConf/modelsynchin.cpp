// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/13/2022 by behm
// ******************************************************

#include "modelsynchin.h"
#include "ipctcpsocket.h"
#include "zconftable.h"

ModelSynchIn::ModelSynchIn(QObject *parent)
	: QObject(parent)
	, m_model(nullptr)
	, m_socket(nullptr)
{

}

void ModelSynchIn::setModel(ZConfTable *model)
{
	m_model = model;
}

void ModelSynchIn::setSocket(IpcTcpSocket *socket)
{
	m_socket = socket;
	connect(socket, &IpcTcpSocket::dataRxd, this, &ModelSynchIn::dataRxd);
}

void ModelSynchIn::dataRxd(int signature, const QByteArray data)
{
	SynchCmd sig = SynchCmd(signature);
//	qDebug() << Q_FUNC_INFO << sig;
	if (! m_model)
	{
		return;
	}
	QDataStream ds(data);
	ZConfItem item;
	ZConfItemVector dv;
	int row1;
	int row2;
	switch (sig)
	{
	case Reset:
		ds >> dv;
//		qDebug() << dv.size() << dv;
		m_model->setData(dv);
		break;
	case Insert:
		ds >> row1 >> row2 >> dv;
//		qDebug() << row1 << row2 << dv.count() << dv;
		m_model->insertRows(row1, row2 - row1 + 1, QModelIndex());
		break;
	case Remove:
		ds >> row1 >> row2;
//		qDebug() << row1 << row2;
		m_model->removeRows(row1, row2 - row1 + 1, QModelIndex());
		break;
	case Update:
		ds >> row1;
		ds >> item;
//		qDebug() << row1 << item;
		m_model->setData(row1, item);
		break;
	}
	emit updated();
}
