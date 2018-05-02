// ******************************************************
// * copyright 2015: (C) by Reinhardt Behm rbehm@hushmail.com
// * All Rights reserved
// * File: meter.cpp    Meter
// * created 1/4/2015 by behm
// ******************************************************

#include "project.h"
#include "meter.h"

Meter::Meter(QWidget *parent)
	: QWidget(parent)
{
	m_precision = 0;
	m_scalePrecision = 0;
	m_steps = 10;
	m_minValue = 0;
	m_maxValue = 100;
	m_startAngle = 215;
	m_endAngle = -25;
	m_thresholdFlag = false;
	m_foreground = QColor(Qt::green);
	m_background = QColor(Qt::darkGray);
	m_numericIndicatorEnabled = true;
//	setMinimumSize(QSize(80,80));
}

void Meter::setValue(qreal value)
{
	qreal lastV = m_value;
	m_value = qBound(m_minValue, value, m_maxValue);
	if (lastV != m_value)
	{
		emit valueChanged(m_value);
	}
	if(m_thresholdEnabled)
	{
		thresholdManager();
	}
	update();
}

void Meter::setMaxValue(const qreal maxValue)
{
	m_maxValue = maxValue;
	update();
}

void Meter::setMinValue(const qreal minValue)
{
	m_minValue = minValue;
	update();
}

void Meter::setRange(const qreal minValue, const qreal maxValue)
{
	m_minValue = minValue;
	m_maxValue = maxValue;
	update();
}

void Meter::setSteps(int steps)
{
	m_steps = steps;
	update();
}

void Meter::setStartAngle(const qreal startAngle)
{
	m_startAngle = startAngle;
	update();
}

void Meter::setEndAngle(const qreal endAngle)
{
	m_endAngle = endAngle;
	update();
}

void Meter::setThreshold(const qreal threshold)
{
	m_threshold = threshold;
	update();
}

void Meter::setThresholdEnabled(bool thresholdEnabled)
{
	m_thresholdEnabled = thresholdEnabled;
	update();
}

void Meter::setNumericIndicatorEnabled(bool numericIndicatorEnabled)
{
	m_numericIndicatorEnabled = numericIndicatorEnabled;
	update();
}

void Meter::setForeground(const QColor foreground)
{
	m_foreground = foreground;
	update();
}

void Meter::setBackground(const QColor background)
{
	m_background = background;
	update();
}

void Meter::setUnits(const QString &units)
{
	m_units = units;
	update();
}

void Meter::setPrecision(int precision)
{
	m_precision = precision;
	update();
}

void Meter::thresholdManager()
{
	if (m_value > m_threshold && !m_thresholdFlag)
	{
		m_thresholdFlag = true;
		emit thresholdAlarm(m_thresholdFlag);
	}
	else if (m_value < m_threshold && m_thresholdFlag)
	{
		m_thresholdFlag = false;
		emit thresholdAlarm(m_thresholdFlag);
	}
}

void Meter::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	int side = qMin(width(), height());
	painter.setViewport((width() - side) / 2, (height() - side) / 2,side, side);
	painter.setWindow(-50, -50, 100, 100);

	drawCrown(&painter);
	drawBackground(&painter);
	drawTicks(&painter);
	drawScale(&painter);
	drawUnits(&painter);
	if (m_thresholdEnabled)
	{
		drawThresholdLine(&painter);
	}
	drawIndicator(&painter);
	if (m_numericIndicatorEnabled)
	{
		drawNumericValue(&painter);
	}
}


void Meter::setScalePrecision(int scalePrecision)
{
	m_scalePrecision = scalePrecision;
	update();
}


void Meter::drawCrown(QPainter *painter)
{
	QRadialGradient haloGradientw(0, 0, 49, 0, 0);
	haloGradientw.setColorAt(0.0, Qt::lightGray);
	haloGradientw.setColorAt(0.8, Qt::darkGray);
	haloGradientw.setColorAt(0.9, Qt::white);
	haloGradientw.setColorAt(1.0, Qt::black);
	painter->setPen(Qt::NoPen);
	painter->setBrush(haloGradientw);
	painter->drawEllipse(-49, -49, 98, 98);
}

void Meter::drawBackground(QPainter *painter)
{
	painter->setBrush(m_background);
	painter->drawEllipse(-45, -45, 90, 90);
}

void Meter::drawTicks(QPainter *painter)
{
	painter->save();
	painter->setPen(m_foreground);
	int nSteps = m_steps;
	painter->rotate(-m_startAngle) ;
	qreal angleStep=(-m_endAngle + m_startAngle) / m_steps;
	for (int i = 0; i <= nSteps * 2; ++i )
	{
		if (i & 0x0001)
		{
			painter->drawLine(22, 0, 24, 0 );
		}
		else
		{
			painter->drawLine(20, 0, 25, 0 );
		}
		painter->rotate(angleStep / 2);
	}
	painter->restore();
}

void Meter::drawScale(QPainter *painter)
{
	painter->save();
	painter->setPen(m_foreground);
	qreal startRad = m_startAngle * M_PI / 180.0;
	startRad += M_PI / 2;
	qreal deltaRad = (m_endAngle - m_startAngle) / qreal(m_steps) * (M_PI / 180);
	for (int i = 0; i <= m_steps; ++i )
	{
		qreal sina = sin(qreal(startRad + i * deltaRad));
		qreal cosa = cos(qreal(startRad + i * deltaRad));
		qreal tmpVal = i * ((m_maxValue - m_minValue) / m_steps);
		tmpVal += m_minValue;
		QString str = QString("%1").arg(tmpVal,0,'f', m_scalePrecision);
		QFontMetricsF fm(this->font());
		QSizeF fs = fm.size(Qt::TextSingleLine, str);
		int x = int((35 * sina) -(fs.width() / 2));
		int y = int((35 * cosa) + (fs.height() / 4));
		painter->drawText(x, y, str);
	}
	painter->restore();
}

void Meter::drawUnits(QPainter *painter)
{
	painter->setPen(m_foreground);
	painter->setBrush(m_foreground);
	QString str(m_units);
	QFont f = font();
	f.setPointSize(8);
	QFontMetricsF fm(f);
	double w = fm.size(Qt::TextSingleLine, str).width();
	QPainterPath path;
	path.addText(QPointF(-w / 2, -10), f, m_units);
	painter->drawPath(path);
}

void Meter::drawIndicator(QPainter *painter)
{
	painter->save();

	painter->rotate(-90.0);
	int size = 3;
	qreal w = 1.8;
	qreal l = 25;
	QPolygonF pts;
	pts << QPoint(w, 0) << QPoint(w, l*0.6) <<  QPoint(0, l) << QPoint(-w, l*0.6) << QPoint(-w, 0);
	int degRotate = int(m_startAngle + (m_endAngle - m_startAngle) / (m_maxValue - m_minValue) * (m_value - m_minValue));

	painter->rotate(-degRotate );
	QRadialGradient haloGradient(0, 0, 20, 0, 0);
	haloGradient.setColorAt(0.0, QColor(120, 120, 250));
	haloGradient.setColorAt(1.0, QColor(20, 20, 160));
	painter->setPen(QPen(Qt::darkBlue, 0));
	painter->setBrush(haloGradient);
	painter->drawConvexPolygon(pts);

	// draw indicator hat
	QColor niceBlue(150, 150, 200);
	QConicalGradient coneGradient(0, 0, -90.0);
	coneGradient.setColorAt(0.0, Qt::darkGray);
	coneGradient.setColorAt(0.2, niceBlue);
	coneGradient.setColorAt(0.5, Qt::white);
	coneGradient.setColorAt(1.0, Qt::darkGray);
	painter->setPen(Qt::NoPen);
	painter->setBrush(coneGradient);
	painter->drawEllipse(-size, -size, 2*size, 2*size);

	painter->restore();
}

void Meter::drawThresholdLine(QPainter *painter)
{
	QColor color(Qt::red);
	color.setAlpha(80);
	QPen pen(color);

	double thresholdAngle = m_startAngle + (m_endAngle - m_startAngle) / (m_maxValue - m_minValue) * (m_threshold - m_minValue);
	pen.setWidth(5);
	painter->setPen(pen);
	painter->drawArc(-22, -22, 44, 44, int(thresholdAngle) * 16, int(-thresholdAngle + m_endAngle) * 16);

}

void Meter::drawNumericValue(QPainter *painter)
{
	QString str = QString("%1").arg(m_value,0, 'f', m_precision);
	QFont f = font();
	QFontMetricsF fm(f);
	double w = fm.size(Qt::TextSingleLine, str).width();
	QColor color = m_thresholdFlag ? Qt::red : color = QColor(150, 150, 200);
	painter->setPen(Qt::NoPen);
	painter->setBrush(color);
	painter->drawRect(-20, 20, 40, 14);
	painter->setBrush(m_background);
	painter->drawRect(-19, 21, 38, 12);
	painter->setPen(m_foreground);
	painter->setBrush(m_foreground);
	QPainterPath path;
	path.addText(QPointF(-w / 2, 31), f, str);
	painter->drawPath(path);
}
