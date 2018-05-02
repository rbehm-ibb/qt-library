// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/3/2017 by behm
// ******************************************************

#ifndef ABSTRACTSCALE_H
#define ABSTRACTSCALE_H

class AbstractScale : public QGraphicsView
{
	Q_OBJECT
public:
	enum Option { None = 0, Vertical = 0x01, Ticks = 0x02 /*, Value = 0x04, Title = 0x08*/ };
	Q_DECLARE_FLAGS(Options, Option)
	class SegmentDescr
	{
	public:
		SegmentDescr(qreal lim, QColor col) : limit(lim), color(col) {}
		qreal limit;
		QColor color;
	};
	typedef QVector<SegmentDescr> SegmentDescrVec;
	explicit AbstractScale(Options opt, QWidget *parent = 0);
	~AbstractScale();
	void setSegmentDescrs(const SegmentDescrVec &sdv);
	void setSegLim(int idx, qreal lim);
	void setSegColor(int idx, QColor color);
	void setHandColor(QColor color);
	void setTick0(qreal x);
	void setTickStep(qreal x, QColor color);
	void setTitle(QString title);
signals:
	void valueChanged(qreal v);

public slots:
	void setValue(qreal v);
	void setMinimum(qreal min);
	void setMaximum(qreal max);
	void setRange(qreal min, qreal max);
private slots:
protected:
	virtual void resizeEvent(QResizeEvent *);
	virtual void update();
	virtual void setSize();
	virtual void createSegments();
	virtual void createTicks();
	virtual void drawHand();
protected:
	const Options m_opt;
	QGraphicsScene *m_scene;
	QLabel *m_valueLabel;
	QLabel *m_titleLabel;
	class ScaleSegment
	{
	public:
		ScaleSegment();
		ScaleSegment(const SegmentDescr &sd);
		qreal limit;
		QColor color;
		QGraphicsPathItem *path;
	};
	QVector<ScaleSegment> m_segments;
	QGraphicsPathItem *m_hand;
	QVector<QGraphicsLineItem*> m_ticks;
	QColor m_handColor;
	QColor m_tickColor;
	qreal m_pathWidth;
	qreal m_min, m_max;
	qreal m_tick0;
	qreal m_tickStep;
	qreal m_value;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(AbstractScale::Options)

#endif // ABSTRACTSCALE_H
