// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/15/2016 by behm
// ******************************************************

#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H


class CsvTableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum Option { NoOption = 0x00, WithHeader = 0x01, Trim = 0x02, Quoted = 0x04 };
	Q_ENUM(Option)
	Q_DECLARE_FLAGS(Options, Option)
	explicit CsvTableModel(QObject *parent = 0);
	~CsvTableModel();
	bool read(const QString dsn, Options options);
	bool save(const QString dsn, Options options) const;
	bool append(const QString dsn, Options options);
	void append(QAbstractTableModel *model);
	QString filename() const { return m_filename; }
	void setHeader(const QStringList &h, bool fixCol);
	void setSep(const QChar &sep) { m_sep = sep; }
	QChar sep() const { return m_sep; }
	void setCodec(const QString codec) { m_codec = codec; }
	QString codec() const { return m_codec; }
	int colNum(const QString col) const { return m_header.indexOf(col); }
	const QStringList requireColumns(const QStringList h) const;	///< returns missing headers
	const QVector<int> columnIdx(const QStringList h) const;

	const QVector<QStringList> &rawData() const { return m_data; }

	// QAbstractItemModel interface
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	virtual bool insertRows(int row, int count, const QModelIndex &parent);
	virtual bool removeRows(int row, int count, const QModelIndex &parent);

	QStringList header() const { return m_header; }

signals:
	void nameChanged(const QString name);
	void error(const QString msg) const;
public slots:
	void clear();
private slots:
protected:
private:
	QStringList m_header;
	QVector<QStringList> m_data;
	QString m_filename;
	int m_colCount;
	QChar m_sep;
	QString m_codec;
	bool open(QFile &f, const QString &name);
	void readData(QTextStream &s, Options options);
	QStringList expandLine(QString line, bool trim);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(CsvTableModel::Options)

#endif // CSVTABLEMODEL_H
