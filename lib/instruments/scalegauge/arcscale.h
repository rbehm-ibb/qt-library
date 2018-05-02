// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/3/2017 by behm
// ******************************************************

#ifndef ARCSCALE_H
#define ARCSCALE_H

#include "abstractscale.h"

class ArcScale : public AbstractScale
{
	Q_OBJECT
public:
	explicit ArcScale(qreal angle, AbstractScale::Options opt = AbstractScale::None, QWidget *parent = 0);
//	~ArcScale();

signals:

public slots:
private slots:
	virtual void setSize();
	virtual void createSegments();
	virtual void createTicks();
	virtual void drawHand();
protected:
private:
	const qreal m_angle;
	qreal m_radius;
	qreal phi(qreal v) { return (90 + m_angle/2 - m_angle * v/(m_max - m_min)); }
};

#endif // ARCSCALE_H
