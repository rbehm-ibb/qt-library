// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/24/2017 by behm
// ******************************************************

#include "scalegauge.h"

ScaleGauge::ScaleGauge(QWidget *parent)
	: QWidget(parent)
	, m_valueLabel(new QLabel(this))
	, m_value(0)
	, m_minimum(0)
	, m_maximum(100)
	, m_limLow(15)
	, m_limHigh(70)
	, m_angle(120 )
{	setFixedSize(100, 100);
	update();
	m_brushLow = QBrush(Qt::blue);
	m_brushMid = QBrush(Qt::green);
	m_brushHigh = QBrush(Qt::red);
	m_brushHand = QBrush(Qt::white);
}

ScaleGauge::~ScaleGauge()
{
}

void ScaleGauge::setColors(QBrush low, QBrush mid, QBrush high, QBrush hand)
{
	m_brushLow = low;
	m_brushMid = mid;
	m_brushHigh = high;
	m_brushHand = hand;
	update();
}

void ScaleGauge::setValue(qreal v)
{
	if (m_value != v)
	{
		m_value = v;
		m_valueLabel->setNum(m_value);
		emit valueChanged(m_value);
		update();
	}
}

void ScaleGauge::setAngle(const qreal &angle)
{
	m_angle = angle ;
	update();
}

void ScaleGauge::setLimHigh(const qreal &limHigh)
{
	m_limHigh = limHigh;
	update();
}

void ScaleGauge::setLimLow(const qreal &limLow)
{
	m_limLow = limLow;
	update();
}

void ScaleGauge::setMaximum(const qreal &maximum)
{
	m_maximum = maximum;
	update();
}

void ScaleGauge::setMinimum(const qreal &minimum)
{
	m_minimum = minimum;
	update();
}

void ScaleGauge::setRange(const qreal &minimum, const qreal &maximum)
{
	m_minimum = minimum;
	m_maximum = maximum;
	update();
}

qreal ScaleGauge::value() const
{
	return m_value;
}


void ScaleGauge::update()
{
	QSize lsz = m_valueLabel->size();
	QRect r = rect();
	int top = r.height() - lsz.height();
	int left = (r.width() - lsz.width()) / 2;
	m_valueLabel->move(left, top);
	repaint();
}

QSize ScaleGauge::minimumSizeHint() const
{
	return QSize(scaleWidth, scaleWidth);
}

void ScaleGauge::resizeEvent(QResizeEvent *)
{
	update();
}

void ScaleGauge::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	// calculate the scale size
	qreal w = width();
	qreal r = (width()-scaleWidth)/2 / sin(m_angle * M_PI / 180. / 2);
	QRect arcRect(0, 0/*scaleWidth/2*/, w, w/*-scaleWidth/2*/);
//	qDebug() << arcRect << m_angle << w << r << h;
	int startPhi = (90 + m_angle/2) * 16;
	int range = m_maximum - m_minimum;
	int lowPhi = m_angle * limLow()/range*16;
	int midPhi = m_angle * limHigh()/range*16;
	int allPhi = m_angle * 16;
	p.setPen(QPen(Qt::red, scaleWidth));
	p.drawArc(arcRect, startPhi, -allPhi);
	p.setPen(QPen(Qt::green, scaleWidth));
	p.drawArc(arcRect, startPhi, -midPhi );
//	qDebug() << startPhi/16 << midPhi/16 << allPhi/16;
	p.setPen(QPen(Qt::blue, scaleWidth));
	p.drawArc(arcRect, startPhi, -lowPhi );
	qreal valuePhi = -m_angle/2 + m_value / range * m_angle;
	QPolygon hand;
	hand << QPoint(-10, 20) << QPoint(10, 20) << QPoint(0, 0);
//	qDebug() << Q_FUNC_INFO << valuePhi << p.transform() << w/2 << r;
	p.translate(w/2, r);
	p.rotate(valuePhi);
	p.translate(0, -(r));
	p.setPen(Qt::white);
	p.drawLine(0, 0, 0, r);
	p.setPen(Qt::NoPen);
	p.setBrush(Qt::white);
	p.drawConvexPolygon(hand);
}
