// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/16/2023 by behm
// ******************************************************

#include "paths.h"

Paths *Paths::m_instance = nullptr;

Paths::Paths(QObject *parent)
	: QObject(parent)
{
	m_instance = this;
	QString tmp;
	QStringList tmps = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
	// qDebug() << Q_FUNC_INFO << "Data" << tmps;
	if (! tmps.isEmpty())
	{
		tmp = tmps.first();
		m_dir = QDir(tmp);
		// qDebug() << Q_FUNC_INFO << ">>>>>>>>>>" << m_dir.absolutePath() << m_dir.exists();
		if (! m_dir.exists())
		{
			/*bool rc = */m_dir.mkpath(m_dir.absolutePath());
			// qDebug() << Q_FUNC_INFO << ">>>>>>>>>>" << rc;
		}
	}
	else if (tmp.isEmpty())
	{
		tmp = qApp->applicationDirPath();
	}
	m_dir = QDir(tmp);
}

bool Paths::ok()
{
	return i()->m_dir.exists();
}

bool Paths::mkdir(const QString &name)
{
	return i()->m_dir.mkdir(name);
}

bool Paths::ok(const QString &name)
{
	return i()->m_dir.exists(name);
}

Paths *Paths::i()
{
	if (! m_instance)
	{
		m_instance = new Paths(qApp);
		//		setValue("app/name", qApp->applicationName());
		//		setValue("app/version", qApp->applicationVersion());
	}
	return m_instance;
}
