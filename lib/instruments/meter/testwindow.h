// ******************************************************
// * copyright 2015: (C) by Reinhardt Behm rbehm@hushmail.com
// * All Rights reserved
// * File: testwindow.h    TestWindow
// * created 1/4/2015 by behm
// ******************************************************

#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "project.h"
#include "meter.h"

class TestWindow : public QWidget
{
	Q_OBJECT
public:
	explicit TestWindow(QWidget *parent = 0);

signals:

public slots:
private slots:
private:
	class Meter * meter;
};

#endif // TESTWINDOW_H
