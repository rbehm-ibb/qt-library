// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 3/3/2017 by behm
// ******************************************************

#ifndef BARSCALE_H
#define BARSCALE_H

#include "abstractscale.h"

class BarScale : public AbstractScale
{
	Q_OBJECT
public:
	explicit BarScale(AbstractScale::Options opt = AbstractScale::Vertical, QWidget *parent = 0);
//	~BarScale();

signals:

public slots:
private slots:

	// AbstractScale interface
protected:
//	virtual void resizeEvent(QResizeEvent *);
//	virtual void update();
	virtual void setSize();
	virtual void createSegments();
	virtual void createTicks();
	virtual void drawHand();
protected:
private:
	qreal m_length;

};

#endif // BARSCALE_H
