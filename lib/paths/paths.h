// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/16/2023 by behm
// ******************************************************

#ifndef PATHS_H
#define PATHS_H

class Paths : public QObject
{
public:
	static bool ok();
	static QDir dataDir() { return i()->m_dir; }
	static QString dataDirname() { return dataDir().absolutePath(); }
	static QString filename(const QString &name) { return dataDir().absoluteFilePath(name); }
	static bool mkdir(const QString &name);
	static bool ok(const QString &name);
private:
	Paths(QObject *parent);
	static Paths *i();
	static Paths *m_instance;
	QDir m_dir;
};

#endif // PATHS_H
