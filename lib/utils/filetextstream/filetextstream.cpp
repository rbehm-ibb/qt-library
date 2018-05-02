// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * File: filetextstream.cpp    FileTextStream
// * created 12/10/2014 by behm
// ******************************************************

#include "filetextstream.h"

FileTextStreamWrite::FileTextStreamWrite(QString filename)
	: m_file(filename)
{
	if (! m_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		qWarning() << Q_FUNC_INFO << m_file.fileName() << m_file.errorString();
	}
	else
	{
		setDevice(&m_file);
	}
}




FileTextStreamRead::FileTextStreamRead(QString filename)
    : m_file(filename)
{
	if (! m_file.open(QIODevice::ReadOnly))
	{
		qWarning() << Q_FUNC_INFO << m_file.fileName() << m_file.errorString();
	}
	else
	{
		setDevice(&m_file);
	}
}


