// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/2/2017 by behm
// ******************************************************

#ifndef MAILATTACHMENT_H
#define MAILATTACHMENT_H

#include "smtpheader.h"

class MailAttachment
{
public:
	MailAttachment();
	friend QTextStream & operator<<(QTextStream &ts, const MailAttachment &d);
	void appendHeader(QByteArray name, QString contents) { m_headers.append(name, contents); }
	void setContents(const QByteArray d) { m_contents = d; }
	void setFile(const QString filename);
	void setType(const QString t);
	void setEncoding(const QString e);
	static QString boundary() { return m_boundary; }

	SmtpHeader headers() const { return m_headers; }


protected:
	SmtpHeader m_headers;
	QString m_type;
	QString m_encoding;
	QByteArray m_contents;
	static QString m_boundary;
private:
};

#endif // MAILATTACHMENT_H
