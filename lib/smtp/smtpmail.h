// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/2/2017 by behm
// ******************************************************

#ifndef SMTPMAIL_H
#define SMTPMAIL_H

#include "smtpheader.h"
#include "mailattachment.h"

class SmtpMail
{
public:
	SmtpMail();
	void setTo(QString to);
	void setFrom(QString from);
	void setReply(QString r);
	void setSubject(const QString &s);
	void addAtachment(const MailAttachment &a) { m_attachments.append(a); }
	void setText(const QString &text) { m_text = text; }
	void setHtml(const QString &text) { m_html = text; }
	friend QTextStream & operator<<(QTextStream &ts, const SmtpMail &d);
	QString to() const;
	QString from() const;
	SmtpHeader headers() const { return m_headers; }

protected:
private:
	SmtpHeader m_headers;
	QVector<MailAttachment> m_attachments;
	QString m_text;
	QString m_html;
};

#endif // SMTPMAIL_H
