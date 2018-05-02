// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/2/2017 by behm
// ******************************************************

#include "mailattachment.h"

QString MailAttachment::m_boundary("AvioScout.boundary.0987654321");

MailAttachment::MailAttachment()
{
}

void MailAttachment::setFile(const QString filename)
{
	QFileInfo fi(filename);
	if (! fi.exists())
	{
		qWarning() << Q_FUNC_INFO << filename << "not found";
		return;
	}
	QFile f(fi.absoluteFilePath());
	if (! f.open(QIODevice::ReadOnly))
	{
		qWarning() << Q_FUNC_INFO << f.fileName() << f.errorString();
		return;
	}
	m_contents = f.readAll();
	appendHeader("Content-Disposition", QString("attachment; filename=\"%1\"").arg(fi.fileName()));
	QMimeDatabase mdb;
	setType(QString("%1; name=\"%2\"").arg(mdb.mimeTypeForFile(fi).name()).arg(fi.fileName()));
	setEncoding("base64");
}

void MailAttachment::setType(const QString t)
{
	m_type = t;
}

void MailAttachment::setEncoding(const QString e)
{
	m_encoding = e;
}


QTextStream & operator<<(QTextStream &ts, const MailAttachment &d)
{
//	qDebug() << Q_FUNC_INFO << d.m_type << d.m_encoding;
	ts << d.m_headers;
	ts << HeaderPair("Content-Type", d.m_type + "; boundary=" + d.boundary());
	ts << HeaderPair("Content-Transfer-Encoding", d.m_encoding);
	ts << "\r\n";
	if (d.m_encoding == "base64")
	{
		ts << d.m_contents.toBase64();
	}
	else
	{
		ts << QString::fromUtf8(d.m_contents);
	}
	ts << "\r\n";
	return ts;
}

