// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/3/2017 by behm
// ******************************************************

#include "arcscale.h"

ArcScale::ArcScale(qreal angle, AbstractScale::Options opt, QWidget *parent)
	: AbstractScale(opt, parent)
	, m_angle(angle)
{
	m_radius = 100;
	QPolygonF poly;
	poly.append(QPointF(0, 0));
	poly.append(QPointF(-m_pathWidth*3, -m_pathWidth));
	poly.append(QPointF(-m_pathWidth*3, m_pathWidth));
	QPainterPath path;
	path.addPolygon(poly);
	m_hand->setPath(path);
	m_hand->setZValue(10);
}

void ArcScale::createSegments()
{
	if (m_radius > 0)
	{
		qreal r = m_radius /*+ m_pathWidth/2*/;
		QRectF rec(QPointF(-r, -r), QPointF(r, r));
		r = m_radius - m_pathWidth;
		QRectF rec2(QPointF(-r, -r), QPointF(r, r));
		qreal phi1 = phi(m_min);
		qreal z = 0;
		foreach (const ScaleSegment &seg, m_segments)
		{
			qreal phi2 = phi(seg.limit);
			QPainterPath path;
			path.arcMoveTo(rec, phi1);
			path.arcTo(rec, phi1, (phi2-phi1));
			QPainterPath path2;
			path2.arcMoveTo(rec2, phi2);
			path2.arcTo(rec2, phi2, -(phi2-phi1));
			path.connectPath(path2);
			path.closeSubpath();
			phi1 = phi2;
			seg.path->setPath(path);
			seg.path->setZValue(z);
			seg.path->setPen(Qt::NoPen);
			seg.path->setBrush(QBrush(seg.color));
			z += 1;
		}
	}
}

void ArcScale::createTicks()
{
	if (m_opt.testFlag(Ticks))
	{
		qDeleteAll(m_ticks);
		m_ticks.clear();
		QLineF line(QPointF(-m_pathWidth*2, 0), QPointF(0, 0));
		QPen pen(m_tickColor, 1);
		for (qreal xt = m_tick0; xt <= m_max; xt += m_tickStep)
		{
			QGraphicsLineItem *li = m_scene->addLine(line, pen);
			m_ticks.append(li);
			QTransform trans;
			trans.rotate(-phi(xt));
			trans.translate(m_radius, 0);
			li->setTransform(trans);
			li->setZValue(10);

		}
	}
}

void ArcScale::drawHand()
{
	m_hand->setBrush(QBrush(m_handColor));
	QTransform trans;
	trans.rotate(-(phi(m_value)));
	trans.translate(m_radius, 0);
	m_hand->setTransform(trans);
	if (m_valueLabel)
	{

	}
}



void ArcScale::setSize()
{
	createSegments();
	createTicks();
	drawHand();
	setSceneRect(m_scene->itemsBoundingRect());
	setMinimumSize(sceneRect().size().toSize());
}

