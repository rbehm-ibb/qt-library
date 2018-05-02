// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/3/2017 by behm
// ******************************************************

#include "abstractscale.h"

AbstractScale::AbstractScale(Options opt, QWidget *parent)
	: QGraphicsView(parent)
	, m_opt(opt)
	, m_scene(new QGraphicsScene(this))
	, m_valueLabel(nullptr)
	, m_titleLabel(nullptr)
	, m_hand(new QGraphicsPathItem)
	, m_pathWidth(10)
	, m_min(0)
	, m_max(100)
	, m_tick0(0)
	, m_tickStep(10)
	, m_value(-1)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setScene(m_scene);
//	QHBoxLayout *hlay = new QHBoxLayout(this);
//	hlay->addStretch(5);
//	QVBoxLayout *lay = new QVBoxLayout;
//	lay->addStretch(5);
//	hlay->addLayout(lay);
//	hlay->addStretch(5);
//	if (m_opt.testFlag(Title))
//	{
//		m_titleLabel = new QLabel(this);
//		m_titleLabel->setObjectName("title");
//		lay->addWidget(m_titleLabel);
//	}
//	if (m_opt.testFlag(Value))
//	{
//		m_valueLabel = new QLabel(this);
//		m_valueLabel->setObjectName("value");
//		lay->addWidget(m_valueLabel);
//	}
	m_handColor = Qt::white;
	m_hand->setPen(Qt::NoPen);
	m_hand->setBrush(QBrush(m_handColor));
	m_hand->setZValue(10);
	m_scene->addItem(m_hand);
}

AbstractScale::~AbstractScale()
{
}

void AbstractScale::setSegmentDescrs(const AbstractScale::SegmentDescrVec &sdv)
{
	foreach (const ScaleSegment &ss, m_segments)
	{
		delete ss.path;
	}
	m_segments.clear();
	m_segments.reserve(sdv.size());
	foreach (const SegmentDescr &sd, sdv)
	{
		ScaleSegment seg(sd);
		seg.path = new QGraphicsPathItem;
		m_scene->addItem(seg.path);
		m_segments.append(seg);
	}
	createSegments();
}

void AbstractScale::setSegLim(int idx, qreal lim)
{
	if (idx >= 0 && idx < m_segments.size())
	{
		m_segments[idx].limit = lim;
	}
	createSegments();
}

void AbstractScale::setSegColor(int idx, QColor color)
{
	if (idx >= 0 && idx < m_segments.size())
	{
		m_segments[idx].color = color;
	}
	createSegments();
}

void AbstractScale::setHandColor(QColor color)
{
	m_handColor = color;
	drawHand();
}

void AbstractScale::setTick0(qreal x)
{
	m_tick0 = x;
	createTicks();
}

void AbstractScale::setTickStep(qreal x, QColor color)
{
	m_tickStep = x;
	m_tickColor = color;
	createTicks();
}

void AbstractScale::setTitle(QString title)
{
	if (m_titleLabel)
	{
		m_titleLabel->setText(title);
		m_titleLabel->adjustSize();
	}
}

void AbstractScale::setValue(qreal v)
{
	if (m_value != v)
	{
		emit valueChanged(v);
		m_value = qBound(m_min, v, m_max);
		if (m_valueLabel)
		{
			m_valueLabel->setNum(v);
		}
		drawHand();
	}
}

void AbstractScale::setMinimum(qreal min)
{
	m_min = min;
	if (! m_segments.isEmpty())
	{
		createSegments();
	}
	createTicks();
	drawHand();
}

void AbstractScale::setMaximum(qreal max)
{
	m_max = max;
	if (! m_segments.isEmpty())
	{
		m_segments.last().limit = max;
		createSegments();
	}
	createTicks();
	drawHand();
}

void AbstractScale::setRange(qreal min, qreal max)
{
	m_min = min;
	setMaximum(max);
}

void AbstractScale::resizeEvent(QResizeEvent *)
{
	setSize();
}

void AbstractScale::update()
{
}

void AbstractScale::setSize()
{
}

void AbstractScale::createSegments()
{
}

void AbstractScale::createTicks()
{
}

void AbstractScale::drawHand()
{
}

AbstractScale::ScaleSegment::ScaleSegment()
	: limit()
	, color(Qt::black)
	, path(nullptr)
{
}

AbstractScale::ScaleSegment::ScaleSegment(const AbstractScale::SegmentDescr &sd)
	: limit(sd.limit)
	, color(sd.color)
	, path(nullptr)
{
}
