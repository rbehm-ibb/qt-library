// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/15/2016 by behm
// ******************************************************

#include "csvtablemodel.h"

CsvTableModel::CsvTableModel(QObject *parent)
	: QAbstractTableModel(parent)
	, m_colCount(0)
	, m_sep(',')
{
}

CsvTableModel::~CsvTableModel()
{
}

bool CsvTableModel::read(const QString dsn, Options options)
{
	auto saveHeader = m_header;
	clear();
	QFile f;
	if (! open(f, dsn))
	{
		m_header = saveHeader;
		m_colCount = m_header.size();
		return false;
	}
	beginResetModel();
	m_header = saveHeader;
	m_colCount = m_header.size();
	QTextStream s(&f);
	if (! m_codec.isEmpty())
	{
		s.setCodec(m_codec.toLocal8Bit());
	}
	QStringList header;
	if (options & WithHeader)
	{
		QString line = s.readLine(1000);
		if (line.isNull())
		{
			QString msg("Error reading header CSV file\n<%1>\n%2");
			emit error(msg.arg(f.fileName()).arg(f.errorString()));
			return false;
		}
		header = expandLine(line.toUpper(), true);
		m_colCount = header.count();
		setHeader(header, options & WithHeader);
	}
	readData(s, options);
	endResetModel();
	emit nameChanged(dsn);
	return true;
}

bool CsvTableModel::save(const QString dsn, Options options) const
{
	QFile f(dsn);
	if (! f.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		QString msg("Error opening CSV file\n<%1>\n%2");
		emit error(msg.arg(f.fileName()).arg(f.errorString()));
		return false;
	}
	QTextStream s(&f);
	if (! m_codec.isEmpty())
	{
		s.setCodec(m_codec.toLocal8Bit());
	}
	if (options & WithHeader)
	{
		s << m_header.join(m_sep) << Qt::endl;
	}
	for (int row = 0; row < rowCount(); ++row)
	{
		QStringList line;
		const QStringList &d = m_data[row];
		for (int col = 0; col < d.count(); ++col)
		{
			if (options & Quoted)
			{
				QString s = d[col];
				s = s.replace("\"", "\"\"");
				line += "\"" + s + "\"";
			}
			else
			{
				line += d[col];
			}
		}
		s << line.join(m_sep) << Qt::endl;
	}
	return true;
}

bool CsvTableModel::append(const QString dsn, Options options)
{
	beginResetModel();
	QFile f;
	if (! open(f, dsn))
	{
		return false;
	}
	QTextStream s(&f);
	readData(s, options & Trim);
	setHeader(m_header, true);	// may need to expand it
	endResetModel();
	return true;
}

void CsvTableModel::append(QAbstractTableModel *model)
{
	beginResetModel();
	//assumes that the structure is the same
	if (m_data.isEmpty())	// take all from model
	{
		m_colCount = model->columnCount();
		m_header.clear();
		for (int col = 0; col < model->columnCount(); ++col)
		{
			m_header << model->headerData(col, Qt::Horizontal).toString();
		}
	}
	for (int row = 0; row < model->rowCount(); ++row)
	{
		QStringList sl;
		for (int col = 0; col < model->columnCount(); ++col)
		{
			QModelIndex idx(model->index(row, col));
			QVariant d = model->data(idx);
			if (! d.isValid() || ! d.canConvert(QVariant::String))
			{
				sl << QString();
			}
			else
			{
				switch (d.type())
				{
				case QVariant::ByteArray:
					sl << QString();
					break;
				default:
					sl << d.toString();
				}
			}
		}
		m_data.append(sl);
	}
	endResetModel();
}

bool CsvTableModel::open(QFile &f, const QString &name)
{
	f.setFileName(name);
	if (! f.open(QIODevice::ReadOnly))
	{
		QString msg("Error opening CSV file\n<%1>\n%2");
		emit error(msg.arg(f.fileName()).arg(f.errorString()));
		return false;
	}
	return true;
}

void CsvTableModel::readData(QTextStream &s, Options options)
{
	forever
	{
		QString line = s.readLine(1000);
		if (line.isNull())
		{
			break;
		}
		if (line.isEmpty())
		{
			continue;
		}
		const QStringList sl = expandLine(line, options & Trim);
		m_data += sl;
		m_colCount = qMax(m_colCount, sl.count());
	}
}

QStringList CsvTableModel::expandLine(QString line, bool trim)
{
	static const QChar dlm('\"');
	QStringList res;
	enum State { Idle, String, DlmString, WaitDlm };
	State state = Idle;

	QString field;
	foreach (const QChar &c, line)
	{
		switch (state)
		{
		case Idle:
			if (c == dlm)
			{
				state = DlmString;
			}
			else if (c == m_sep)
			{
				res += "";
			}
			else if (! c.isSpace())
			{
				field = c;
				state = String;
			}
			break;
		case String:
			if (c == m_sep)
			{
				res += field.trimmed();
				state = Idle;
				field.clear();
			}
			else
			{
				field += c;
			}
			break;
		case DlmString:
			if (c == dlm)
			{
				res += field;
				state = WaitDlm;
				field.clear();
			}
			else
			{
				field += c;
			}
			break;
		case WaitDlm:
			if (c == m_sep)
			{
				state = Idle;
			}
			break;
		}
	}
	if (state == DlmString)
	{
		emit error("Unterminated field in CSV file\n" + line);
	}
	else if (state == String)
	{
		if (trim)
		{
			res += field.simplified();
		}
		else
		{
			res += field;
		}
	}
	return res;
}

void CsvTableModel::clear()
{
	beginResetModel();
	m_data.clear();
	m_colCount = 0;
	m_header.clear();
	endResetModel();
	if (! m_filename.isEmpty())
	{
		m_filename = QString();
		emit nameChanged(m_filename);
	}
}

void CsvTableModel::setHeader(const QStringList &h, bool fixCol)
{
	m_header = h;
	if (fixCol)
	{
		m_colCount = h.count();
	}
	else
	{
		while (m_header.count() < m_colCount)
		{
			m_header += QString("Col_%1").arg(m_header.count());
		}
	}
//	qDebug() << Q_FUNC_INFO << m_header << m_colCount;
	emit headerDataChanged(Qt::Horizontal, 0, m_colCount);
}

const QStringList CsvTableModel::requireColumns(const QStringList h) const
{
	QStringList res;
	foreach (const QString &s, h)
	{
		if (m_header.indexOf(s) < 0)
		{
			res += s;
		}
	}
	return res;
}

const QVector<int> CsvTableModel::columnIdx(const QStringList h) const
{
	QVector<int> res;
	foreach (const QString &s, h)
	{
		int idx  = m_header.indexOf(s.toUpper());
		res += idx;
	}
	return res;
}

int CsvTableModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.count();
}

int CsvTableModel::columnCount(const QModelIndex &/*parent*/) const
{
	return m_colCount;
}

bool CsvTableModel::hasChildren(const QModelIndex &/*parent*/) const
{
	return false;
}

QVariant CsvTableModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid() && index.row() < m_data.count() && index.column() < m_colCount)
	{
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
		{
			const QStringList &sl = m_data[index.row()];
			if (index.column() < sl.count())
			{
				return sl[index.column()];
			}
			else
			{
				return QString("");
			}
		}
		break;
		}
	}
	return QVariant();
}

QVariant CsvTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (section < m_colCount && orientation == Qt::Horizontal)
		{
			if (section >= m_header.size())
			{
				return QString("Col_%1(%2,%3)").arg(section).arg(m_header.size()).arg(m_colCount);
			}
			return m_header[section];
		}
		else if (orientation == Qt::Vertical)
		{
			return section;
		}
	}
	return QVariant();
}

Qt::ItemFlags CsvTableModel::flags(const QModelIndex &/*index*/) const
{
	Qt::ItemFlags flags =  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
	return flags;
}


bool CsvTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
	row = qBound(0, row, rowCount());
	QStringList dummy;
	beginInsertRows(parent, row, row + count - 1);
	m_data.insert(row, count, dummy);
	endInsertRows();
	return true;
}

bool CsvTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
	row = qBound(0, row, rowCount() - 1);

	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
//		qDebug() << Q_FUNC_INFO << i << m_data[row].cursor;
		m_data.removeAt(row);
	}
	endRemoveRows();
	return true;
}
