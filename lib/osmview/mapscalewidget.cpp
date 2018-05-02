// ******************************************************
// * copyright (C) 2014 by Becker Electronics Taiwan Ltd.
// * All Rights reserved
// * File: mapscalewidget.cpp    MapScaleWidget
// * created 9/29/2014 by behm
// ******************************************************

#include "mapscalewidget.h"

MapScaleWidget::MapScaleWidget(QWidget *parent)
	: QLabel(parent)
	, m_scale(0)
	, font("helvetica", 12, QFont::Bold)
{
	setStyleSheet("* { margin: 0px; padding: 0px; border: 0; } ");
}

void MapScaleWidget::changedScale(QTransform trans)
{
	qreal scale = 1. / qAbs(trans.m22());
	if (m_scale != scale)
	{
//		qDebug() << Q_FUNC_INFO << trans << m_scale;
		m_scale = scale;
		createImage();
	}
}

void MapScaleWidget::createImage()
{
	static const double EARTH_MEAN_RADIUS = 6371.0072;
	if (m_scale <= 0)	// invalid
	{
		return;
	}
	struct Units
	{
		qreal lim;
		const char * text;
	};
	static const Units segmet[] =
	{
		{ 1e7, "10000km" },
		{ 1e6, "1000km" },
		{ 1e5, "100km" },
		{ 1e4, "10km" },
		{ 1e3, "1km" },
		{ 1e2, "100m" },
		{ 1e1, "10m" },
		{ 1e0, "1m" },
		{ 0, 0 }
	};
	qreal scale = m_scale * EARTH_MEAN_RADIUS * 2 * M_PI / 360. * 1000.;	// m/pix
	int len = (480 / 2);	// max len
	qreal mlen = len * scale;	// max len in m

	const Units * pUnit;
	for (pUnit = segmet ; pUnit->text != 0; ++pUnit)
	{
		if (mlen / 2 > pUnit->lim)
		{
			break;
		}
	}
//	qDebug() << Q_FUNC_INFO << scale << m_scale << len << mlen << pUnit->lim << pUnit->text;

	const QString text(pUnit->text);
	const int OFFSET = 5;
	const int W = 4;
	QFontMetrics fm(font);
	QSize st = fm.boundingRect(text).size() + QSize(2*OFFSET, 0);
	QPixmap pix(QSize(2*OFFSET+W + st.height(), 2*OFFSET+len));	// size of the picture
	QPixmap pixtext(st);
	QPainter p(&pixtext);		// the painter on the picture
	QColor color(192, 192, 255);
//	color.setAlpha(0);
	p.setPen(Qt::NoPen);
	p.setBrush(color);

	p.setBrush(color);
	p.drawRect(0, 0, st.width(), st.height());
	p.setFont(font);
	p.setPen(Qt::black);
	p.drawText(pixtext.rect(), text);
	p.end();

	pix.fill(QColor(0, 0, 0, 0));

	p.begin(&pix);
	p.setPen(Qt::NoPen);
	color.setAlpha(0);
	p.setBrush(color);
	p.drawRect(0, 0, 2*OFFSET+W+2, 2*OFFSET+len);

	int y0 = 0;
	int i = 0;
	bool white = true;
	qreal seglen = pUnit->lim;
	int xpos = OFFSET+W/2 + 3;
	while (y0 < len)
	{
		int y1 = int(qreal(i) * seglen / scale);
		if (y1 > len)
			break;
		if (white)
		{
			p.setPen(QPen(Qt::white, W));
		}
		else
		{
			p.setPen(QPen(Qt::black, W));
		}
		p.drawLine(xpos, OFFSET+y0, xpos, OFFSET+y1);
		y0 = y1;
		++i;
		white = ! white;
	}
	y0 = 0;
	i = 0;
	white = true;
	seglen *= 1.853;
	xpos -= W + 2;
	while (y0 < len)
	{
		int y1 = int(qreal(i) * seglen / scale);
		if (y1 > len)
			break;
		if (white)
		{
			p.setPen(QPen(Qt::white, W));
		}
		else
		{
			p.setPen(QPen(Qt::blue, W));
		}
		p.drawLine(xpos, OFFSET+y0, xpos, OFFSET+y1);
		y0 = y1;
		++i;
		white = ! white;
	}

	p.translate(OFFSET+W+3 + pixtext.height(), 0);
	p.rotate(90);
	p.drawPixmap(0, 0, pixtext);
	p.end();
	setPixmap(pix);
	setFixedSize(pix.size());
}
