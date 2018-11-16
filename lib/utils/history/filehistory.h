// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/26/2018 by behm
// ******************************************************

#ifndef FILEHISTORY_H
#define FILEHISTORY_H


class FileHistory : public QObject
{
	Q_OBJECT
public:
	explicit FileHistory(QWidget *parent = 0);
	~FileHistory();

	QAction *histAction() const { return m_histAction; }

	int maxHist() const { return m_maxHist; }
	void setMaxHist(int maxHist);

	QStringList history() const { return m_history; }

	QString groupKey() const { return m_groupKey; }
	void setGroupKey(const QString &groupKey);

	QString arrayKey() const { return m_arrayKey; }
	void setArrayKey(const QString &arrayKey);

	QString itemKey() const { return m_itemKey; }
	void setItemKey(const QString &itemKey);

signals:
	void historySelected(const QString &s);
public slots:
	void addHistory(const QString &s);
	void addHistory(QFile *f);
private slots:
	void historySlot();
protected:
private:
	void limitAndStore();
	void readHistory();
	QAction *m_histAction;
	QMenu *m_menu;
	int m_maxHist;
	QStringList m_history;
	QString m_groupKey;
	QString m_arrayKey;
	QString m_itemKey;

};

#endif // FILEHISTORY_H
