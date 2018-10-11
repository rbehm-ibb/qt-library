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
	explicit CsvTableModel(QObject *parent = 0);
	~CsvTableModel();
	bool read(const QString dsn, bool withHeaders);
	bool save(const QString dsn, bool withHeaders);
	bool append(const QString dsn);
	void append(QAbstractTableModel *model);
	QString filename() const { return m_filename; }
	void setHeader(const QStringList &h, bool fixCol);
	void setSep(const QChar &sep) { m_sep = sep; }
	QChar sep() const { return m_sep; }
	void setCodec(const QString codec) { m_codec = codec; }
	QString codec() const { return m_codec; }

	const QVector<QStringList> &rawData() const { return m_data; }

	// QAbstractItemModel interface
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;

signals:
	void nameChanged(const QString name);
	void error(const QString msg);
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
	void readData(QTextStream &s);
	QStringList expandLine(QString line);
};

#endif // CSVTABLEMODEL_H
