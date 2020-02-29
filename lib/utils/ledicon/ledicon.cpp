// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// * ,
// ******************************************************

#include "ledicon.h"

LedIcon::LedIcon(QWidget *parent)
	: QLabel(parent)
	, m_on(false)
{
	m_color = LedIconSource::Green;
	QPixmap pm = LedIconSource::pix(LedIconSource::On);
	m_size = pm.width();
	setLed(LedIconSource::Off);
}

LedIcon::LedIcon(LedIconSource::LedColor color, QWidget *parent)
	: QLabel(parent)
	, m_on(false)
{
	setLed(color);
}

void LedIcon::setLed(LedIconSource::LedColor color)
{
	setPixmap(LedIconSource::pix(color).scaled(m_size, m_size));
	m_color = color;
}

void LedIcon::setOnOff(bool on)
{
	m_on = on;
	const QPixmap pm(on ? LedIconSource::pix(m_color) : LedIconSource::pix(false));
	setPixmap(pm.scaled(m_size, m_size));
}

void LedIcon::toggle()
{
	setOnOff(! m_on);
}

void LedIcon::setSize(int size)
{
	m_size = size;
	setLed(m_color);
}



