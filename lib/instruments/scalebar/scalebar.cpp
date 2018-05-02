// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/23/2017 by behm
// ******************************************************

#include "scalebar.h"

ScaleBar::ScaleBar(Qt::Orientation orientation, QWidget *parent)
	: QWidget(parent)
	, m_orientation(orientation)
{
//	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
	m_limitLow = 20;
	m_limitHigh = 70;
	m_tick0 = 0;
	m_tickStep = 10;
	QLinearGradient gr(QPointF(0,0), QPointF(0.5,0));
	gr.setCoordinateMode(QGradient::ObjectBoundingMode);
	gr.setSpread(QGradient::ReflectSpread);
	gr.setColorAt(0, QColor("#0000ff"));
	gr.setColorAt(0.47, QColor("#0000ff"));
	gr.setColorAt(0.5, QColor("#0000ff"));
	m_lowColor = QBrush(gr);
	gr.setColorAt(0, QColor("#00ff00"));
	gr.setColorAt(0.47, QColor("#00ff00"));
	gr.setColorAt(0.5, QColor("#00ff00"));
	m_midColor = QBrush(gr);
	gr.setColorAt(0, QColor("#ff0000"));
	gr.setColorAt(0.47, QColor("#ff0000"));
	gr.setColorAt(0.5, QColor("#ff0000"));
	m_highColor = QBrush(gr);
	m_markerColor = Qt::white;
	m_minimum = 0;
	m_maximum = 100;
}

ScaleBar::~ScaleBar()
{
}

void ScaleBar::paintEvent(QPaintEvent *)
{
//	qDebug() << Q_FUNC_INFO;
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	qreal h = height();
	qreal w = width();
//	QSize sz = size();
	QPen pen = p.pen();
	p.setPen(Qt::NoPen);
	qreal v = qBound(m_minimum, m_value, m_maximum);
	if (m_orientation == Qt::Vertical)
	{
		qreal bw = w - markerSize;
		qreal yl = (m_limitLow - m_minimum) / (m_maximum - m_minimum);
		int iyl = int(h * yl);
		p.setBrush(m_midColor);
		p.drawRect(0, 0, bw, h);
		if (iyl > 0)
		{
			p.setBrush(m_lowColor);
			p.drawRect(0, h-iyl, bw, h);
		}
		qreal yh = (m_limitHigh - m_minimum) / (m_maximum - m_minimum);
		int iyh = int(h * yh);
		if (iyh > 0)
		{
			p.setBrush(m_highColor);
			p.drawRect(0, 0, bw, h-iyh);
		}
		int iy = int((v - m_minimum) / (m_maximum - m_minimum) * h);
		int left = bw/2;
		if (iy >= 0 && iy <= h)
		{
			iy = h - iy;
			QPolygonF poly;
			poly << QPointF(left, iy) << QPointF(w, iy - markerSize) << QPointF(w, iy + markerSize);
			p.setBrush(m_markerColor);
			p.setPen(QPen(Qt::black, 2));
			p.drawPolygon(poly);
		}
		p.setPen(pen);
		for (qreal ys = m_tick0; ys <= m_maximum; ys += m_tickStep)
		{
			int iy = int((ys - m_minimum) / (m_maximum - m_minimum) * h);
			if (iy >= 0 && iy <= h)
			{
				p.drawLine(left, h-iy, w-markerSize/2, h-iy);
			}

		}
	}
}

void ScaleBar::setLimitHigh(const qreal &limitHigh)
{
	m_limitHigh = limitHigh;
	update();
}

void ScaleBar::setLimitLow(const qreal &limitLow)
{
	m_limitLow = limitLow;
	update();
}


QSize ScaleBar::sizeHint() const
{
	if (m_orientation == Qt::Vertical)
		return QSize(markerSize + Width, Width);
	else
		return QSize(Width, markerSize + Width);
}

void ScaleBar::setRange(qreal min, qreal max)
{
	m_minimum = min;
	m_maximum = max;
	update();
}

void ScaleBar::setColors(QColor low, QColor mid, QColor high, QColor marker)
{
	m_lowColor = low;
	m_midColor = mid;
	m_highColor = high;
	m_markerColor = marker;
	update();
}

void ScaleBar::setColors(QBrush low, QBrush mid, QBrush high, QBrush marker)
{
	m_lowColor = low;
	m_midColor = mid;
	m_highColor = high;
	m_markerColor = marker;
	update();
}

void ScaleBar::setValue(qreal v)
{
	if (m_value != v)
	{
		m_value = v;
		emit valueChanged(m_value);
		update();
	}
}

void ScaleBar::resizeEvent(QResizeEvent *)
{
	update();
}


void ScaleBar::setTickStep(const qreal &tickStep)
{
	m_tickStep = tickStep;
	update();
}



void ScaleBar::setTick0(const qreal &tick0)
{
	m_tick0 = tick0;
	update();
}


void ScaleBar::update()
{
	//	qDebug() << Q_FUNC_INFO << m_minimum << m_maximum << m_value << m_limitLow << m_limitHigh;
	repaint();
}



ScaleBarDisplay::ScaleBarDisplay(Options opt, QWidget *parent)
	: QWidget(parent)
	, m_bar(new ScaleBar(opt.testFlag(Vertical) ? Qt::Vertical : Qt::Horizontal, this))
	, m_title(nullptr)
	, m_value(new QLCDNumber(this))
{
	m_value->setObjectName("value");
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(0);
	QHBoxLayout *hlayout = new QHBoxLayout;
	if (! opt.testFlag(NoTitle))
	{
		m_title = new QLabel(this);
		m_title->setObjectName("title");
		layout->addWidget(m_title);
	}
//	m_title->setVisible(false);
	layout->addLayout(hlayout);
	layout->addWidget(m_value);
	hlayout->addStretch(1);
	hlayout->addWidget(m_bar);
	hlayout->addStretch(1);
}

void ScaleBarDisplay::setTitle(const QString &title)
{
	if (m_title)
	{
		m_title->setText(title);
		m_title->setAlignment(Qt::AlignCenter);
	}
}

void ScaleBarDisplay::setUnit(const QString &unit)
{
	m_unit = unit;
	setDisplay(value());
}

void ScaleBarDisplay::setValue(qreal v)
{
	m_bar->setValue(v);
	setDisplay(v);
}

void ScaleBarDisplay::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ScaleBarDisplay::setDisplay(qreal v)
{
//	QString txt = m_unit.isEmpty() ? QString("%1").arg(v) : QString("%1%2").arg(v).arg(m_unit);
	m_value->display(v);
//	m_value->setAlignment(Qt::AlignRight);
}
