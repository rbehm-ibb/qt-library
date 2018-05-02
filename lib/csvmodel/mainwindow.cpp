// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * File: mainwindow.cpp    MainWindow
// * created 2/7/2015 by behm
// ******************************************************

#include "mainwindow.h"
#include "toolbarspacer.h"
#include "csvtablemodel.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
      , m_model(new CsvTableModel(this))
{
	setWindowIcon(QIcon(":/wizard"));
	setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());

	QToolBar *toolbar = addToolBar("tb");
	QAction *actExit = toolbar->addAction(QIcon(":/exit"), tr("&Quit"), this, SLOT(quit()));
	actExit->setShortcut(QKeySequence::Quit);
	QAction *actOpen = toolbar->addAction(QIcon(":/fileopen"), tr("Open"), this, SLOT(open()));
	actOpen->setShortcut(QKeySequence::Open);
	toolbar->addAction(QIcon(":/add"), "Append", this, SLOT(append()));
	toolbar->addAction(QIcon(":/remove"), tr("Clear"), m_model, SLOT(clear()));

	ToolBarSpacer::addSpacer(toolbar);
	toolbar->addAction(QIcon(":/info"), tr("&About"), this, SLOT(about()));

	statusBar()->addPermanentWidget(m_fileLabel = new QLabel);

	QTableView *view = new QTableView;
	setCentralWidget(view);

	view->setModel(m_model);

	connect(m_model, &CsvTableModel::error, this, &MainWindow::error);
	connect(m_model, &CsvTableModel::nameChanged, this, &MainWindow::nameChanged);
}

void MainWindow::quit()
{
	close();
}

void MainWindow::open()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
							qApp->applicationFilePath(),
							tr("CSV (*.csv);;All Files (*)"));
	if (! filename.isEmpty())
	{
		m_model->read(filename, true);
	}
}

void MainWindow::append()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Append File"),
							qApp->applicationFilePath(),
							tr("CSV (*.csv);;All Files (*)"));
	if (! filename.isEmpty())
	{
		m_model->append(filename+"_");
	}
}

void MainWindow::nameChanged(const QString name)
{
	m_fileLabel->setText(name);
}

void MainWindow::error(const QString msg)
{
	QMessageBox::warning(this, "Warning", msg);
}

void MainWindow::about()
{
	char year[] = "2016";
	QString text("<h1>%1</h1>"
		     "<p>Version %2"
		     "<p>&copy; %3, %4"
		     "<p>Web: <a href=\"http://%5\">http://%5</a>"
		     "<p>Mail: <a href=\"mailto:info@%5\">info@%5</a>"
		     "<p>Using  <img src=\":/stdicons/qt-logo-about.png\"> %6"
		     );
	text = text
		.arg(qApp->applicationName())
		.arg(qApp->applicationVersion())
		.arg(year)
		.arg(qApp->organizationName())
		.arg(qApp->organizationDomain())
		.arg(qVersion())
		;
	QMessageBox::about(this, qApp->applicationName(), text);
}
