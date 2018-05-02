// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/23/2017 by behm
// ******************************************************

#ifndef SCALEBAR_H
#define SCALEBAR_H


class ScaleBar : public QWidget
{
	Q_OBJECT
public:
	explicit ScaleBar(Qt::Orientation orientation = Qt::Vertical, QWidget *parent = 0);
	~ScaleBar();
	virtual QSize sizeHint() const;

	void setRange(qreal min,  qreal max);
	void setColors(QColor low, QColor mid, QColor high, QColor marker);
	void setColors(QBrush low, QBrush mid, QBrush high, QBrush marker);

	qreal limitLow() const { return m_limitLow; }
	qreal limitHigh() const { return m_limitHigh; }
	qreal value() const { return m_value; }

	qreal tick0() const { return m_tick0; }
	void setTick0(const qreal &tick0);

	qreal tickStep() const { return m_tickStep; }
	void setTickStep(const qreal &tickStep);

signals:
	void valueChanged(qreal v);
public slots:
	void setValue(qreal v);
	void setLimitHigh(const qreal &limitHigh);
	void setLimitLow(const qreal &limitLow);
private slots:
protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent *);
private:
	qreal m_minimum;
	qreal m_maximum;
	qreal m_limitLow;
	qreal m_limitHigh;
	qreal m_value;
	qreal m_tick0;
	qreal m_tickStep;
	QBrush m_lowColor;
	QBrush m_midColor;
	QBrush m_highColor;
	QBrush m_markerColor;
	Qt::Orientation m_orientation;
	void update();
	static constexpr int markerSize = 10;
	static constexpr int Width = 10;
};

class ScaleBarDisplay : public QWidget
{
	Q_OBJECT
public:
	enum Option { None, Vertical = 1, NoTitle = 2 };
	Q_DECLARE_FLAGS(Options, Option)
	ScaleBarDisplay(Options opt, QWidget *parent = 0);
	void setRange(qreal min,  qreal max) { m_bar->setRange(min, max); }
	void setColors(QColor low, QColor mid, QColor high, QColor marker) { m_bar->setColors(low, mid, high, marker); }
	void setColors(QBrush low, QBrush mid, QBrush high, QBrush marker) { m_bar->setColors(low, mid, high, marker); }

	qreal limitLow() const { return m_bar->limitLow(); }
	qreal limitHigh() const { return m_bar->limitHigh(); }
	qreal value() const { return m_bar->value(); }

	qreal tick0() const { return m_bar->tick0(); }
	void setTick0(const qreal &tick0) { m_bar->setTick0(tick0); }

	qreal tickStep() const { return m_bar->tickStep(); }
	void setTickStep(const qreal &tickStep) { m_bar->setTickStep(tickStep); }
	QString unit() const { return m_unit; }
	void setTitle(const QString &title);
	void setUnit(const QString &unit);

signals:
	void valueChanged(qreal v);
public slots:
	void setValue(qreal v);
	void setLimHigh(const qreal &limitHigh) { m_bar->setLimitHigh(limitHigh); }
	void setLimLow(const qreal &limitLow) { m_bar->setLimitLow(limitLow); }
private slots:
protected:
	virtual void paintEvent(QPaintEvent *);
private:
	ScaleBar *m_bar;
	QLabel *m_title;
	QLCDNumber *m_value;
	QString m_unit;
	void setDisplay(qreal v);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ScaleBarDisplay::Options)

#endif // SCALEBAR_H
