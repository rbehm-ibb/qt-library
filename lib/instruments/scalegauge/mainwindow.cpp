// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/22/2017 by behm
// ******************************************************

#include "mainwindow.h"
#include "barscale.h"
#include "arcscale.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(qApp->applicationName() + " " +qApp->applicationVersion());
	QToolBar *toolbar = addToolBar("tb");
	QAction *actExit = toolbar->addAction(QIcon(":/exit"), tr("&Quit"), this, SLOT(close()));
	actExit->setShortcut(QKeySequence::Quit);
	QDoubleSpinBox *spin = new QDoubleSpinBox;
	toolbar->addWidget(spin);

	vb = new BarScale(/*AbstractScale::Vertical |*/ AbstractScale::Ticks);
	hb = new ArcScale(120, AbstractScale::Ticks);
	vb->setHandColor(Qt::magenta);
	hb->setHandColor(Qt::white);
	vb->setSegmentDescrs(AbstractScale::SegmentDescrVec
			     ( {
				       AbstractScale::SegmentDescr(25, Qt::yellow),
				       AbstractScale::SegmentDescr(70, Qt::green),
				       AbstractScale::SegmentDescr(100, Qt::red)
			       }
			       )
			     );
	hb->setSegmentDescrs(AbstractScale::SegmentDescrVec
			     ( {
				       AbstractScale::SegmentDescr(25, Qt::green),
				       AbstractScale::SegmentDescr(60, Qt::yellow),
				       AbstractScale::SegmentDescr(70, Qt::red)
			       }
			       )
			     );
	vb->setRange(0, 100);
	hb->setRange(0, 100);
	vb->setTick0(0);
	vb->setTickStep(20, Qt::white);
	hb->setTick0(0);
	hb->setTickStep(20, Qt::white);
	hb->setTitle("RPM");
	connect(spin, SIGNAL(valueChanged(double)), vb, SLOT(setValue(qreal)));
	connect(spin, SIGNAL(valueChanged(double)), hb, SLOT(setValue(qreal)));
	QSplitter *cw = new QSplitter;
	setCentralWidget(cw);
	cw->addWidget(vb);
	cw->addWidget(hb);
	spin->setValue(50);
	QDoubleSpinBox *spinL = new QDoubleSpinBox;
	toolbar->addWidget(spinL);
	connect(spinL, SIGNAL(valueChanged(double)), this, SLOT(chLow(qreal)));
	QDoubleSpinBox *spinH = new QDoubleSpinBox;
	toolbar->addWidget(spinH);
	connect(spinH, SIGNAL(valueChanged(double)), this, SLOT(chHigh(qreal)));
}

void MainWindow::chLow(qreal v)
{
	vb->setSegLim(0, v);
	hb->setSegLim(0, v);
}

void MainWindow::chHigh(qreal v)
{
	vb->setSegLim(1, v);
	hb->setSegLim(1, v);
}

