// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/26/2018 by behm
// ******************************************************

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "filehistory.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
      , m_hist(new FileHistory(this))
{
	ui->setupUi(this);
	ui->toolBar->addAction(m_hist->histAction());
	ui->menuFile->addAction(ui->actionOpen);
	ui->menuFile->addAction(m_hist->histAction());
	ui->menuFile->addAction(ui->toolBar->quit());
	ui->menuHelp->addAction(ui->toolBar->about());

	connect(m_hist, &FileHistory::historySelected, this, &MainWindow::historySelected);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::quit()
{
	close();
}

void MainWindow::historySelected(const QString &s)
{
	qDebug() << Q_FUNC_INFO << s;
	openFile(s);
}

void MainWindow::on_actionOpen_triggered()
{
	qDebug() << Q_FUNC_INFO;
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
							Config::stringValue("/file/last", qApp->applicationFilePath()),
							tr("All Files (*)"));
	if (! fileName.isEmpty())
	{
		openFile(fileName);
	}
}

void MainWindow::openFile(const QString &fn)
{
	m_hist->addHistory(fn);
	Config::setValue("/file/last", fn);
	QFile f(fn);
	if (f.open(QIODevice::ReadOnly))
	{
		QString s = f.readAll();
		ui->textBrowser->setText(s);
	}
}
