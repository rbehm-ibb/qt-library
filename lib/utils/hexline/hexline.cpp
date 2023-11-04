// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/16/2023 by behm
// ******************************************************

#include "hexline.h"

HexLine::HexLine(QWidget *parent)
	: QLineEdit(parent)
	, m_len(0)
{
	QAction *cpy = addAction(QIcon("://source_h.svgz"), QLineEdit::TrailingPosition);
	QAction *ascCpy = addAction(QIcon("://font.svgz"), QLineEdit::TrailingPosition);
	QAction *format = addAction(QIcon("://accept.png"), QLineEdit::TrailingPosition);
	setClearButtonEnabled(true);
	QFont font;
	font.setFamily(QString::fromUtf8("Oxygen Mono"));
//	font.setPointSize(12);
	setFont(font);
//	connect(this, &QLineEdit::returnPressed, this, &HexLine::endEdit);
	connect(cpy, &QAction::triggered, this, &HexLine::getHex);
	connect(ascCpy, &QAction::triggered, this, &HexLine::getAscii);
	connect(format, &QAction::triggered, this, &HexLine::endEdit);
}

void HexLine::setData(ByteVector d)
{
	if (m_len)
	{
		d.expandTo(m_len);
	}
	setText(d.toHex(','));
}

ByteVector HexLine::data()
{
	QString txt = text();
	txt.replace("0x", "");
	ByteVector d(QByteArray::fromHex(txt.toLocal8Bit()));
	if (m_len)
	{
		d.expandTo(m_len);
	}
	setData(d);
	return d;
}

void HexLine::endEdit()
{
	QString txt = text();
	txt.replace("0x", "");
	setText(QByteArray::fromHex(txt.toLocal8Bit()).toHex(','));
}

void HexLine::getHex()
{
	endEdit();
	ByteVector d = data();
	QString s = d.toBA().toHex();
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(s);
}

void HexLine::getAscii()
{
	endEdit();
	ByteVector d = data();
	QString asc;
	foreach (const uint8_t c, d)
	{
		asc += QChar(c);
	}
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(asc);
}
