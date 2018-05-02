// ******************************************************
// * copyright 2015: (C) by Reinhardt Behm rbehm@hushmail.com
// * All Rights reserved
// * File: meter.h    Meter
// * created 1/4/2015 by behm
// ******************************************************

#ifndef METER_H
#define METER_H

#include "project.h"
#include <QWidget>

class Meter : public QWidget
{
	Q_OBJECT
	//	Q_PROPERTY(double value READ value WRITE setValue)
	//	Q_PROPERTY(double minValue READ minValue WRITE setMinValue)
	//	Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue)
	//	Q_PROPERTY(double threshold READ threshold WRITE setThreshold)
	//	Q_PROPERTY(QString format READ format WRITE setFormat)
public:
	explicit Meter(QWidget *parent = 0);

	qreal value() const { return m_value; }

	qreal maxValue() const { return m_maxValue; }
	void setMaxValue(const qreal maxValue);

	qreal minValue() const { return m_minValue; }
	void setMinValue(const qreal minValue);
	void setRange(const qreal minValue, const qreal maxValue);

	int steps() const { return m_steps; }
	void setSteps(int steps);

	qreal startAngle() const { return m_startAngle; }
	void setStartAngle(const qreal startAngle);

	qreal endAngle() const { return m_endAngle; }
	void setEndAngle(const qreal endAngle);

	qreal threshold() const { return m_threshold; }
	void setThreshold(const qreal threshold);

	bool thresholdFlag() const { return m_thresholdFlag; }

	bool thresholdEnabled() const { return m_thresholdEnabled; }
	void setThresholdEnabled(bool thresholdEnabled);

	bool numericIndicatorEnabled() const { 	return m_numericIndicatorEnabled; }
	void setNumericIndicatorEnabled(bool numericIndicatorEnabled);

	QColor foreground() const { return m_foreground; }
	void setForeground(const QColor foreground);

	QColor background() const { return m_background; }
	void setBackground(const QColor background);

	int precision() const { 	return m_precision; }
	void setPrecision(int precision);

	int scalePrecision() const { return m_scalePrecision; }
	void setScalePrecision(int scalePrecision);

	QString units() const { return m_units; }
	void setUnits(const QString &units);

signals:
	void valueChanged(qreal value);
	void thresholdAlarm(bool thresholdFlag);
public slots:
	void setValue(qreal value);
protected:
	qreal m_value;
	qreal m_maxValue;
	qreal m_minValue;
	void thresholdManager();
	void paintEvent(QPaintEvent *event);
	int m_precision;
	int m_scalePrecision;
	QString m_units;
	int m_steps;
	qreal m_startAngle;
	qreal m_endAngle;
	qreal m_threshold;
	bool m_thresholdFlag;

	bool m_thresholdEnabled;
	bool m_numericIndicatorEnabled;

	QColor m_foreground;
	QColor m_background;

	void drawCrown(QPainter *painter);
	void drawBackground(QPainter *painter);
	void drawTicks(QPainter *painter);
	void drawScale(QPainter *painter);
	void drawUnits(QPainter *painter);
	void drawIndicator(QPainter *painter);
	void drawThresholdLine(QPainter *painter);
	void drawNumericValue(QPainter *painter);

};

#endif // METER_H
