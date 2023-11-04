// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/16/2023 by behm
// ******************************************************

#ifndef HEXLINE_H
#define HEXLINE_H

#include <QLineEdit>
#include <QObject>
#include "bytevector.h"

class HexLine : public QLineEdit
{
public:
	HexLine(QWidget *parent = nullptr);
	void setData(ByteVector d);
	ByteVector data();
	int len() const
	{
		return m_len;
	}
	void setLen(int newLen)
	{
		m_len = newLen;
	}
private slots:
	void endEdit();
	void getHex();
	void getAscii();

private:
	int  m_len;
};

#endif // HEXLINE_H
