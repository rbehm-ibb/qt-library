// *************************************
// * Created by model-builder, V1.10
// * Fri Jun 10 10:10:40 2022 by behm
// *************************************

#include "zconftable.h"

ZConfTable::ZConfTable(QObject *parent)
	: QAbstractTableModel(parent)
	, m_header( { "Name", "Serial", "Ip", "Port", "Stamp" })
, m_stdRoles( { Qt::DisplayRole, Qt::EditRole })
, m_maxAge(0)
{
	startTimer(1000);
}

ZConfTable::~ZConfTable()
{
}

void ZConfTable::clear()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}

void ZConfTable::setData(const ZConfItemVector &d)
{
	beginResetModel();
	m_data = d;
	endResetModel();
}

void ZConfTable::setData(int row, const ZConfItem &d)
{
	if (row >= 0 && row < rowCount())
	{
		m_data[row] = d;
		emit dataChanged(index(row, 0), index(row, NCols - 1), m_stdRoles);
	}
}

void ZConfTable::addItem(const ZConfItem &d)
{
//	qDebug() << Q_FUNC_INFO << d;
	int irow = -1;
	for (int i =  0; i < rowCount(); ++i)
	{
		const ZConfItem &dd = m_data[i];
		if ((dd.name == d.name) && (dd.serial == d.serial) && (dd.ip == d.ip) && (dd.port == d.port))
		{
			irow = i;
//			qDebug() << Q_FUNC_INFO << i << "found";
			break;
		}
	}
	if (irow < 0)
	{
		insertRow(0);
		irow = 0;
	}
	setData(irow, d);
}

bool ZConfTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && index.row() < m_data.count())
	{
		auto &t = m_data[index.row()];
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
			switch (index.column())
			{
			case Name:
			{
				const QString v = value.toString();
				if (v != t.name)
				{
					t.name = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			case Serial:
			{
				const QString v = value.toString();
				if (v != t.serial)
				{
					t.serial = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			case Ip:
			{
				const QString v = value.toString();
				if (v != t.ip)
				{
					t.ip = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			case Port:
			{
				const qint16 v = value.toInt();
				if (v != t.port)
				{
					t.port = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			case Stamp:
			{
				const QDateTime v = value.toDateTime();
				if (v != t.stamp)
				{
					t.stamp = v;
					emit dataChanged(index, index, m_stdRoles);
				}
				return true;
			}
			}
		}
	}
	return false;
}

QVariant ZConfTable::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			if (section <= columnCount())
			{
				return m_header[section];
			}
		}
		else if (orientation == Qt::Vertical)
		{
			return section;
		}
	}
	return QVariant();
}

QVariant ZConfTable::data(const QModelIndex &index, int role) const
{
	if (index.isValid() && index.row() < m_data.count())
	{
		const auto &t = m_data[index.row()];
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
			switch (index.column())
			{
			case Name:
				return t.name;
			case Serial:
				return t.serial;
			case Ip:
				return t.ip;
			case Port:
				return t.port;
			case Stamp:
				return t.stamp.toString(Qt::ISODate);
			}
			break;
		}
	}
	return QVariant();
}

bool ZConfTable::insertRows(int row, int count, const QModelIndex &parent)
{
	row = qBound(0, row, rowCount());
	ZConfItem dummy;
	beginInsertRows(parent, row, row + count - 1);
	m_data.insert(row, dummy);
	endInsertRows();
	return true;
}

bool ZConfTable::removeRows(int row, int count, const QModelIndex &parent)
{
	row = qBound(0, row, rowCount());
	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
		m_data.removeAt(row);
	}
	endRemoveRows();
	return true;
}

QDebug operator<<(QDebug dbg, const ZConfItem &d)
{
	return dbg
	       << " ZConfItem["
	       << d.name << ','
	       << d.serial << ','
	       << d.ip << ','
	       << d.port << ','
	       << d.stamp
	       << "]";
}


QDataStream &operator<<(QDataStream &s, const ZConfItem &d)
{
	return s
	       << d.name
	       << d.serial
	       << d.ip
	       << d.port
	       << d.stamp
	       ;
}


QDataStream &operator>>(QDataStream &s, ZConfItem &d)
{
	return s
	       >> d.name
	       >> d.serial
	       >> d.ip
	       >> d.port
	       >> d.stamp
	       ;
}

void ZConfTable::saveConf(QSettings *setting) const
{
	setting->beginGroup("ZConfTable");
	setting->beginWriteArray("data", rowCount());
	for (int row = 0; row < rowCount(); ++row)
	{
		setting->setArrayIndex(row);
		setting->setValue("name", m_data[row].name);
		setting->setValue("serial", m_data[row].serial);
		setting->setValue("ip", m_data[row].ip);
		setting->setValue("port", m_data[row].port);
		setting->setValue("stamp", m_data[row].stamp);
	}
	setting->endArray();
	setting->endGroup();
}

void ZConfTable::loadConf(QSettings *setting)
{
	beginResetModel();
	setting->beginGroup("ZConfTable");
	int n = setting->beginReadArray("data");
	m_data.clear();
	for (int i = 0; i < n; ++i)
	{
		ZConfItem d;
		setting->setArrayIndex(i);
		d.name = setting->value("name").toString();
		d.serial = setting->value("serial").toString();
		d.ip = setting->value("ip").toString();
		d.port = setting->value("port").toInt();
		d.stamp = setting->value("stamp").toDateTime();
		m_data.append(d);
	}
	setting->endArray();
	setting->endGroup();
	endResetModel();
}


void ZConfTable::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event)
//	qDebug() << Q_FUNC_INFO << m_maxAge;
	if (m_maxAge)
	{
		QDateTime ct = QDateTime::currentDateTime();
		for (int i = 0; i < rowCount(); ++i)
		{
			if (m_data[i].port > 0)
			{
				const int dt = m_data[i].stamp.secsTo(ct);
				if (dt > m_maxAge)
				{
//					qDebug() << Q_FUNC_INFO << m_maxAge << i << dt;
					removeRow(i);
				}
			}
		}
	}
}
