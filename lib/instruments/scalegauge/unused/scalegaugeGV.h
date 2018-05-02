// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/1/2017 by behm
// ******************************************************

#ifndef SCALEGAUGE_H
#define SCALEGAUGE_H

class ScaleGauge : public QGraphicsView
{
	Q_OBJECT
public:
	explicit ScaleGauge(QWidget *parent = 0);
	~ScaleGauge();

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
	QGraphicsScene *m_scene;
	QGraphicsPathItem *m_arcL;
	QGraphicsPathItem *m_arcM;
	QGraphicsPathItem *m_arcH;
	QGraphicsPolygonItem *m_hand;
	qreal m_angle;
	qreal m_limLow, m_limHigh;
	qreal m_min, m_max;
	qreal m_value;
	static constexpr qreal scaleWidth = 10;
	static constexpr qreal handSize = 10;
	void setup();
};

#endif // SCALEGAUGEGV_H
