// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/2/2017 by behm
// ******************************************************

#ifndef SMTPHEADER_H
#define SMTPHEADER_H

class HeaderPair : public QPair<QString, QString>
{
public:
	HeaderPair() : QPair<QString, QString>(QString(), QString()) {}
	HeaderPair(QString key, QString value) : QPair<QString, QString>(key, value) {}
	friend QTextStream & operator<<(QTextStream &ts, const HeaderPair d);
	const QString key() const { return first; }
	const QString value() const { return second; }
};

class SmtpHeader
{
public:
	SmtpHeader() {}
	void clear() { m_headers.clear(); }
	friend QTextStream & operator<<(QTextStream &ts, const SmtpHeader d);
	void append(QString name, QString contents) { m_headers.insert(name, HeaderPair(name, contents)); }
	const QMap<QString, HeaderPair> headers() const { return m_headers; }
	const QString value(const QString &k) const { return m_headers.value(k).value(); }

protected:
private:
	QMap<QString, HeaderPair> m_headers;
};

#endif // SMTPHEADER_H
