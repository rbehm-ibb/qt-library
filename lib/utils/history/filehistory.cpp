// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/26/2018 by behm
// ******************************************************

#include "filehistory.h"
#include "config.h"


FileHistory::FileHistory(QWidget *parent)
	: QObject(parent)
	, m_histAction(new QAction(QIcon(":/list.png"), tr("History"), this))
	, m_menu(new QMenu(parent))
	, m_maxHist(5)
	, m_groupKey("/file")
	, m_arrayKey("history")
	, m_itemKey("file")
{
	connect(m_histAction, &QAction::triggered, this, &FileHistory::historySlot);
	m_histAction->setShortcut(QKeySequence("Alt+H"));
	readHistory();
}

FileHistory::~FileHistory()
{
}

void FileHistory::historySlot()
{
//	qDebug() << Q_FUNC_INFO << sender();
	m_menu->clear();
	foreach (const QString &s, m_history)
	{
		m_menu->addAction(s);
	}
	m_menu->addSeparator();
	QAction *cancel = m_menu->addAction(QIcon("://cancel.png"), tr("Cancel"));
	QWidget *pw = qobject_cast<QWidget*>(parent());
	Q_ASSERT(pw != nullptr);
	QAction *act = m_menu->exec(pw->geometry().topLeft());
//	qDebug() << Q_FUNC_INFO << act << (act ? act->text() : QString("###"));
	if (act && (act != cancel))
	{
//		qDebug() << Q_FUNC_INFO << act->text();
		emit historySelected(act->text());
	}
}

void FileHistory::addHistory(const QString &s)
{
//	qDebug() << Q_FUNC_INFO << s;
	m_history.prepend(s);
	limitAndStore();
}

void FileHistory::addHistory(QFile *f)
{
	addHistory(f->fileName());
}

void FileHistory::limitAndStore()
{
//	qDebug() << Q_FUNC_INFO << m_history;
	m_history.removeDuplicates();
	while (m_history.size() > m_maxHist)
	{
		m_history.removeLast();
	}
//	qDebug() << Q_FUNC_INFO << m_history;
	QSettings *conf = Config::conf();
	conf->beginGroup(m_groupKey);
	conf->beginWriteArray(m_arrayKey, m_history.size());
	for (int i = 0; i < m_history.size(); ++i)
	{
		conf->setArrayIndex(i);
		conf->setValue(m_itemKey, m_history[i]);
	}
	conf->endArray();
	conf->endGroup();
	conf->sync();
}

void FileHistory::readHistory()
{
	m_history.clear();
	QSettings *conf = Config::conf();
	conf->beginGroup(m_groupKey);
	int n = conf->beginReadArray(m_arrayKey);
	for (int i = 0; i < n; ++i)
	{
		conf->setArrayIndex(i);
		QString fn = conf->value(m_itemKey).toString();
		m_history << fn;
	}
	conf->endArray();
	conf->endGroup();
	if (m_maxHist  < n)
	{
		m_maxHist = n;
	}
}


void FileHistory::setItemKey(const QString &itemKey)
{
	m_itemKey = itemKey;
	readHistory();
}

void FileHistory::setArrayKey(const QString &arrayKey)
{
	m_arrayKey = arrayKey;
	readHistory();
}

void FileHistory::setGroupKey(const QString &groupKey)
{
	m_groupKey = groupKey;
	readHistory();
}

void FileHistory::setMaxHist(int maxHist)
{
	m_maxHist = maxHist;
	limitAndStore();
}




