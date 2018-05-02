// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/2/2017 by behm
// ******************************************************

#include "smtpheader.h"

QTextStream &operator<<(QTextStream &ts, const SmtpHeader d)
{
	foreach (const HeaderPair &h, d.m_headers)
	{
		ts << h;
	}
	return ts;
}

QTextStream & operator<<(QTextStream &ts, const HeaderPair d)
{
	return ts << d.key() << ": " << d.value() << "\r\n";
}
