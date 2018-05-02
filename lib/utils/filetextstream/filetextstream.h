// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * File: filetextstream.h    FileTextStream
// * created 12/10/2014 by behm
// ******************************************************

#ifndef FILETEXTSTREAM_H
#define FILETEXTSTREAM_H

class FileTextStreamWrite : public QTextStream
{
public:
	FileTextStreamWrite(QString filename);
	const QFile &file() const { return m_file; }

private:
	QFile m_file;
};

class FileTextStreamRead : public QTextStream
{
public:
	FileTextStreamRead(QString filename);
	const QFile &file() const { return m_file; }

private:
	QFile m_file;
};

#endif // FILETEXTSTREAM_H
