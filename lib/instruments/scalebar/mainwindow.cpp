// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/22/2017 by behm
// ******************************************************

#include "mainwindow.h"
#include "scalebar.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(qApp->applicationName() + " " +qApp->applicationVersion());
	QToolBar *toolbar = addToolBar("tb");
	QAction *actExit = toolbar->addAction(QIcon(":/exit"), tr("&Quit"), this, SLOT(close()));
	actExit->setShortcut(QKeySequence::Quit);
	QDoubleSpinBox *spin = new QDoubleSpinBox;
	toolbar->addWidget(spin);

	ScaleBarDisplay * cw = new ScaleBarDisplay(ScaleBarDisplay::Vertical);
	cw->setTitle("Test Scale");
	connect(spin, SIGNAL(valueChanged(double)), cw, SLOT(setValue(qreal)));
	spin->setValue(50);
	setCentralWidget(cw);
	QDoubleSpinBox *spinL = new QDoubleSpinBox;
	toolbar->addWidget(spinL);
	connect(spinL, SIGNAL(valueChanged(double)), cw, SLOT(setLimLow(qreal)));
	QDoubleSpinBox *spinH = new QDoubleSpinBox;
	toolbar->addWidget(spinH);
	connect(spinH, SIGNAL(valueChanged(double)), cw, SLOT(setLimHigh(qreal)));
}

