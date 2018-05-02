// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/3/2017 by behm
// ******************************************************

#include "barscale.h"

BarScale::BarScale(AbstractScale::Options opt, QWidget *parent)
	: AbstractScale(opt, parent)
	, m_length(-1)
{
	QPolygonF poly;
	if (m_opt.testFlag(Vertical))
	{
		poly.append(QPointF(0, 0));
		poly.append(QPointF(20, 10));
		poly.append(QPointF(20, -10));
	}
	else
	{
		poly.append(QPointF(0, 0));
		poly.append(QPointF(10, 20));
		poly.append(QPointF(-10, 20));
	}
	QPainterPath path;
	path.addPolygon(poly);
	m_hand->setPath(path);
	m_hand->setPen(Qt::NoPen);
	m_hand->setZValue(10);
}

void BarScale::createSegments()
{
	if (m_length > 0)
	{
		qreal range = m_max - m_min;
		qreal x = 0;
		bool vert = m_opt.testFlag(Vertical);
		qreal z = 0;
		QPointF p0(0, 0);
		if (vert)
		{
			p0.setY(m_length - x);
		}
		else
		{
			p0.setX(x);
		}
		foreach (const ScaleSegment &seg, m_segments)
		{
			QPainterPath path;
			qreal x1 = (seg.limit - m_min) / range * m_length;
			QPointF p1(0, 0);
			if (vert)
			{
				p0.setX(-m_pathWidth/2);
				p1.setX(m_pathWidth/2);
				p1.setY(m_length - x1);
			}
			else
			{
				p0.setY(-m_pathWidth/2);
				p1.setY(m_pathWidth/2);
				p1.setX(x1);
			}
			path.addRect(QRectF(p0, p1));
			p0 = p1;
			seg.path->setPath(path);
			seg.path->setZValue(z);
			seg.path->setPen(Qt::NoPen);
			seg.path->setBrush(QBrush(seg.color));
			z += 1;
		}
	}
}

void BarScale::createTicks()
{
	if (m_opt.testFlag(Ticks))
	{
		qDeleteAll(m_ticks);
		m_ticks.clear();
		bool vert = m_opt.testFlag(Vertical);
		qreal range = m_max - m_min;
		QLineF line(QPointF(-m_pathWidth/2, 0), QPointF(m_pathWidth/2, 0));
		if (! vert)
		{
			line = QLineF(QPointF(0, -m_pathWidth/2), QPointF(0, m_pathWidth/2));
		}
		QPen pen(m_tickColor, 1);
		for (qreal xt = m_tick0; xt <= m_max; xt += m_tickStep)
		{
			QGraphicsLineItem *li = m_scene->addLine(line, pen);
			m_ticks.append(li);
			qreal x = xt / range * m_length;
			QTransform trans;
			trans.translate(vert ? 0 : x, vert ? m_length - x : 0);
			li->setTransform(trans);
			li->setZValue(10);

		}
	}
}

void BarScale::drawHand()
{
	m_hand->setBrush(QBrush(m_handColor));
	bool vert = m_opt.testFlag(Vertical);
	qreal range = m_max - m_min;
	qreal x = (qBound(m_min, m_value, m_max) - m_min) / range * m_length;
	QTransform trans;
	trans.translate(vert ? 0 : x, vert ? m_length - x : 0);
	m_hand->setTransform(trans);
//	qDebug() << Q_FUNC_INFO << trans;

	setSceneRect(m_scene->itemsBoundingRect());
}

void BarScale::setSize()
{
	if (m_opt.testFlag(Vertical))
	{
		m_length = height();
	}
	else
	{
		m_length = width();
	}
	createSegments();
	createTicks();
	drawHand();
}
