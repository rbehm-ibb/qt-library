// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/1/2017 by behm
// ******************************************************

#include "scalegauge.h"

ScaleGauge::ScaleGauge(QWidget *parent)
	: QGraphicsView(parent)
	, m_valueLabel(new QLabel(this))
	, m_scene(new QGraphicsScene(this))
	, m_arcL(0)
	, m_angle(120 )
	, m_limLow(15)
	, m_limHigh(70)
	, m_min(0)
	, m_max(100)
	, m_value(0)
{
	setScene(m_scene);
	m_scene->setBackgroundBrush(Qt::black);
	QGraphicsPathItem *ell0 = new QGraphicsPathItem;
	ell0->setZValue(-10);
	ell0->setPen(QPen(Qt::black, 1));
	ell0->setBrush(Qt::NoBrush);
	qreal r = 100;
	QRectF rec(QPointF(-r, -r), QPointF(r, r));
	QPainterPath path1;
	path1.arcMoveTo(rec, 0);
	path1.arcTo(rec, 0, 360);
	ell0->setPath(path1);
	m_scene->addItem(ell0);
	m_arcL = new QGraphicsPathItem;
	m_arcL->setZValue(0);
	m_arcL->setPen(QPen(Qt::yellow, scaleWidth));
	m_arcL->setBrush(Qt::NoBrush);
	m_arcM = new QGraphicsPathItem;
	m_arcM->setZValue(1);
	m_arcM->setPen(QPen(QColor("#00ff00"), scaleWidth));
	m_arcM->setBrush(Qt::NoBrush);
	m_arcH = new QGraphicsPathItem;
	m_arcH->setZValue(2);
	m_arcH->setPen(QPen(QColor("#ff0000"), scaleWidth));
	m_arcH->setBrush(Qt::NoBrush);
	m_scene->addItem(m_arcL);
	m_scene->addItem(m_arcM);
	m_scene->addItem(m_arcH);
	QPolygonF poly( { QPointF(0,10), QPointF(0,-10), QPointF(20,0) } );
	QPainterPath pathH;
	m_hand = new QGraphicsPolygonItem(poly);
	m_hand->setZValue(3);
//	hand->setPen(QPen(Qt::black));
	m_hand->setBrush(Qt::white);
	m_scene->addItem(m_hand);
	setup();
}

ScaleGauge::~ScaleGauge()
{
}

void ScaleGauge::setValue(qreal v)
{
	if (m_value != v)
	{
		m_value = v;
		m_valueLabel->setNum(m_value);
		emit valueChanged(m_value);
		setup();
	}
}

void ScaleGauge::setMinimum(const qreal &minimum)
{
	m_min = minimum;
	setup();
}

void ScaleGauge::setMaximum(const qreal &maximum)
{
	m_max = maximum;
	setup();
}

void ScaleGauge::setLimLow(const qreal &limLow)
{
	m_limLow = limLow;
	setup();
}

void ScaleGauge::setLimHigh(const qreal &limHigh)
{
	m_limHigh = limHigh;
	setup();
}

void ScaleGauge::setup()
{
	qreal r = 100;
	QRectF rec(QPointF(-r, -r), QPointF(r, r));
	QPainterPath path1;
	m_angle = 120;
	qreal start = 90 + m_angle/2;
	path1.arcMoveTo(rec, start);
	path1.arcTo(rec, start, -m_angle);
	m_arcL->setPath(path1);
	qreal range = m_max - m_min;
	qreal lowPhi = m_angle * m_limLow/range;
	qreal midPhi = m_angle * m_limHigh/range;
	QPainterPath path2;
	path2.arcMoveTo(rec, start-lowPhi);
	path2.arcTo(rec, start-lowPhi, lowPhi - m_angle);
	m_arcM->setPath(path2);
	QPainterPath path3;
	path3.arcMoveTo(rec, start-midPhi);
	path3.arcTo(rec, start-midPhi, midPhi-m_angle);
	m_arcH->setPath(path3);
	qreal valPhi = m_angle * m_value/range;
	QTransform trans;
	trans.rotate(-(start-valPhi));
	trans.translate(r-10, 0);
	m_hand->setTransform(trans);

	setSceneRect(m_arcL->boundingRect());
	QSize lsz = m_valueLabel->size();
	QRect sr = rect();
	int top = sr.height() - lsz.height();
	int left = (sr.width() - lsz.width()) / 2;
	m_valueLabel->move(left, top);
}

