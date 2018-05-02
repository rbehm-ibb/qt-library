// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/24/2017 by behm
// ******************************************************

#ifndef SCALEGAUGE_H
#define SCALEGAUGE_H


class ScaleGauge : public QWidget
{
	Q_OBJECT
public:
	explicit ScaleGauge(QWidget *parent = 0);
	~ScaleGauge();
	void setColors(QBrush low, QBrush mid, QBrush high, QBrush hand);

	qreal value() const;

	void setRange(const qreal &minimum, const qreal &maximum);
	qreal minimum() const { return m_minimum; }

	qreal maximum() const { return m_maximum; }

	qreal limLow() const { return m_limLow; }

	qreal limHigh() const { return m_limHigh; }

	qreal angle() const { return m_angle; }
	void setAngle(const qreal &angle);
signals:
	void valueChanged(qreal v);
public slots:
	void setValue(qreal v);
	void setMinimum(const qreal &minimum);
	void setMaximum(const qreal &maximum);
	void setLimLow(const qreal &limLow);
	void setLimHigh(const qreal &limHigh);
private slots:
protected:
private:
	QLabel *m_valueLabel;
	qreal m_value;
	qreal m_minimum;
	qreal m_maximum;
	qreal m_limLow;
	qreal m_limHigh;
	qreal m_angle;
	QBrush m_brushLow;
	QBrush m_brushMid;
	QBrush m_brushHigh;
	QBrush m_brushHand;
	static constexpr int scaleWidth = 10;
	static constexpr int handSize = 10;
	void update();

	// QWidget interface
public:
	virtual QSize minimumSizeHint() const;

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent *);
};

#endif // SCALEGAUGE_H
