// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/19/2018 by behm
// ******************************************************

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
//	ui->toolBar->removeWhatis();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::quit()
{
	close();
}
