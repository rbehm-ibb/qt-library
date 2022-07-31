// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/13/2022 by behm
// ******************************************************

#include "modelsynchout.h"
#include "ipctcpserver.h"
#include "ipctcpsocket.h"
#include "zconftable.h"

ModelSynchOut::ModelSynchOut(IpcTcpServer *parent)
	: QObject(parent)
	, m_model(nullptr)
	, m_server(parent)
{
	connect(m_server, &IpcTcpServer::connectedSignal, this, &ModelSynchOut::fullUpdate);
	connect(this, &ModelSynchOut::send, m_server, &IpcTcpServer::send);
}

void ModelSynchOut::setModel(ZConfTable *model)
{
	m_model = model;
	connect(model, &ZConfTable::modelReset, this, &ModelSynchOut::modelReset);
	connect(model, &ZConfTable::rowsInserted, this, &ModelSynchOut::rowsInserted);
	connect(model, &ZConfTable::rowsRemoved, this, &ModelSynchOut::rowsRemoved);
	connect(model, &ZConfTable::dataChanged, this, &ModelSynchOut::dataChanged);
	modelReset();
}

void ModelSynchOut::dataRxd(int signature, const QByteArray data)
{
	Q_UNUSED(signature)
	Q_UNUSED(data);
}

void ModelSynchOut::fullUpdate(IpcTcpSocket *socket)
{
	if (m_model)
	{
		QByteArray ba;
		QDataStream ds(&ba, QIODevice::WriteOnly);
		ds << m_model->data();
		socket->send(Reset, ba);
	}
}

void ModelSynchOut::modelReset()
{
//	qDebug() << Q_FUNC_INFO;
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);
	ds << m_model->data();
	emit send(Reset, ba);
}

void ModelSynchOut::rowsInserted(const QModelIndex &parent, int first, int last)
{
	Q_UNUSED(parent)
//	qDebug() << Q_FUNC_INFO << first << last;
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);
	ZConfItemVector dv;
	for  (int row = first; row <= last; ++row)
	{
		dv << m_model->data(row);
	}
	ds << first << last << dv;
	emit send(Insert, ba);
}

void ModelSynchOut::rowsRemoved(const QModelIndex &parent, int first, int last)
{
	Q_UNUSED(parent)
//	qDebug() << Q_FUNC_INFO << first << last;
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);
	ds << first << last;
	emit send(Remove, ba);
}

void ModelSynchOut::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
	Q_UNUSED(roles)
//	qDebug() << Q_FUNC_INFO << topLeft << bottomRight;
	QByteArray ba;
	QDataStream ds(&ba, QIODevice::WriteOnly);
	Q_ASSERT(topLeft.row() == bottomRight.row());	// allow  only from one row
	ds << topLeft.row();
	ds << m_model->data(topLeft.row());
	emit send(Update, ba);

}
