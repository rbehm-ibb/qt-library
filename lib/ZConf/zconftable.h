// *************************************
// * Created by model-builder, V1.10
// * Fri Jun 10 10:10:40 2022 by behm
// *************************************

#ifndef ZCONFTABLE_H
#define ZCONFTABLE_H

struct ZConfItem
{
	ZConfItem() {}
	friend QDebug operator<<(QDebug dbg, const ZConfItem &d);
	friend QDataStream & operator<<(QDataStream &s, const ZConfItem &d);
	friend QDataStream & operator>>(QDataStream &s, ZConfItem &d);
	QString name;
	QString serial;
	QString ip;
	int port;
	QDateTime stamp;
};
typedef QVector<ZConfItem> ZConfItemVector;
Q_DECLARE_METATYPE(ZConfItem)

class ZConfTable : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum Colums { Name, Serial, Ip, Port, Stamp, NCols };
	Q_ENUMS(Colums)
	explicit ZConfTable(QObject *parent = nullptr);
	~ZConfTable() override;
	void saveConf(QSettings *setting) const;
	void loadConf(QSettings *setting);
	void setMaxAge(int max) { m_maxAge = max; }
	virtual int rowCount(const QModelIndex & = QModelIndex()) const  override { return m_data.count(); }
	virtual int columnCount(const QModelIndex & = QModelIndex()) const  override { return NCols; }
	virtual bool hasChildren(const QModelIndex & = QModelIndex()) const  override { return false; }
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual Qt::ItemFlags flags(const QModelIndex & = QModelIndex()) const  override { return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEditable; }
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	void clear();
	virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
	virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
	const ZConfItemVector & data() const { return m_data; }
	void setData(const ZConfItemVector &d);
	void setData(int row, const ZConfItem &d);
	const ZConfItem & data(int row) const { return m_data.at(row); }
	void addItem(const ZConfItem &d);
private:
	const QStringList m_header;
	const QVector<int> m_stdRoles;
	ZConfItemVector m_data;
	int m_maxAge;

	// QObject interface
protected:
	void timerEvent(QTimerEvent *event) override;
};

#endif // ZCONVTABLE_H
