// ******************************************************
// * copyright 2015: (C) by Reinhardt Behm rbehm@hushmail.com
// * All Rights reserved
// * File: testwindow.cpp    TestWindow
// * created 1/4/2015 by behm
// ******************************************************

#include "project.h"
#include "testwindow.h"

TestWindow::TestWindow(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout * layout = new QHBoxLayout(this);
	QFormLayout *form = new QFormLayout;
	meter = new Meter(this);
	layout->addWidget(meter);
	layout->addLayout(form);
	QDoubleSpinBox *value = new QDoubleSpinBox;
	form->addRow("Value", value);
	QDoubleSpinBox *result = new QDoubleSpinBox;
	form->addRow("Result", result);
	connect(value, SIGNAL(valueChanged(double)), meter, SLOT(setValue(qreal)));
	connect(meter, SIGNAL(valueChanged(qreal)), result, SLOT(setValue(double)));
	meter->setThreshold(35);
	meter->setThresholdEnabled(true);
	meter->setUnits("V");
	meter->setMinimumSize(300, 300);
	meter->setRange(0, 40);
	meter->setSteps(4);
	meter->setPrecision(1);
	meter->setScalePrecision(0);
	meter->setNumericIndicatorEnabled(false);
}
