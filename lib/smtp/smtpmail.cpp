// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/2/2017 by behm
// ******************************************************

#include "smtpmail.h"

SmtpMail::SmtpMail()
{
}

void SmtpMail::setTo(QString to)
{
	m_headers.append("To", to);
}

void SmtpMail::setFrom(QString from)
{
	m_headers.append("From", from);
}

void SmtpMail::setReply(QString r)
{
	m_headers.append("Reply-To", r);
}

void SmtpMail::setSubject(const QString &s)
{
	m_headers.append("Subject", s);
}

QString SmtpMail::to() const
{
	return m_headers.value("To");
}

QString SmtpMail::from() const
{
	return m_headers.value("From");
}


QTextStream & operator<<(QTextStream &ts, const SmtpMail &d)
{
//	qDebug() << Q_FUNC_INFO << d.m_text.size() << d.m_html.size() << d.m_attachments.size();
	SmtpHeader h = d.m_headers;
	h.append("Date", QDateTime::currentDateTime().toString(Qt::RFC2822Date));
	h.append("MIME-Version", "1.0");
	if (d.m_attachments.isEmpty() && d.m_html.isEmpty())
	{
		h.append("Content-Type", "text/plain; charset=\"utf-8\"");
		ts << h << "\r\n";
		QString txt(d.m_text);
		txt.replace("\n", "\r\n");
		ts << txt.toUtf8() << "\r\n";
	}
	if (d.m_attachments.isEmpty() && ! d.m_html.isEmpty())	// with html
	{
		if (d.m_text.isEmpty())
		{
			h.append("Content-Type", "text/html; charset=\"utf-8\"");
			ts << h;
			ts << d.m_html.toUtf8() << "\r\n";
		}
		else	// text + html
		{
			MailAttachment att0;
			att0.setContents(QByteArray());
			att0.setEncoding("7bit");
			att0.setType("multipart/alternative");
			MailAttachment att1;
			QString txt(d.m_text);
			txt.replace("\n", "\r\n");
			att1.setContents(txt.toUtf8());
			att1.setEncoding("8bit");
			att1.setType("text/plain; charset=\"utf-8\"");
			MailAttachment att2;
			att2.setContents(d.m_html.toLatin1());
			att2.setEncoding("7bit");
			att2.setType(" text/html; charset=\"us-ascii\"");
			ts << h << att0
			   << "\r\n--" << MailAttachment::boundary() << "\r\n"
			   << att1
			   << "\r\n--" << MailAttachment::boundary() << "\r\n"
			   << att2
			   << "\r\n--" << MailAttachment::boundary() << "--\r\n"
			      ;
		}
	}
	if (! d.m_attachments.isEmpty()	)
	{
//		qDebug() << Q_FUNC_INFO << "attachments";
		MailAttachment att0;
		att0.setType("text/plain; charset=\"utf-8\"");
		att0.setEncoding("8Bit");
		QString txt(d.m_text);
		txt.replace("\n", "\r\n");
		att0.setContents(txt.toUtf8());
		h.append("Content-Type", "multipart/mixed; boundary=" + MailAttachment::boundary());
		ts << h << "\r\n\r\n--" << MailAttachment::boundary() << "\r\n" << att0 << "--" << MailAttachment::boundary() << "\r\n";
		for (int i = 0; i < d.m_attachments.count()-1; ++i)
		{
			ts << d.m_attachments[i] << "--" << MailAttachment::boundary() << "\r\n";
		}
		ts << d.m_attachments.last() << "--" << MailAttachment::boundary() << "--\r\n";
	}
	return ts;
}

