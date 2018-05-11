// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * File: mainwindow.cpp    MainWindow
// * created 2/7/2015 by behm
// ******************************************************

#include "project.h"
#include "mainwindow.h"
#include "toolbarspacer.h"
#include "ledicon.h"
#include "ledbutton.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setWindowIcon(QIcon(":/wizard"));
	setWindowTitle(qApp->applicationName() + " " +qApp->applicationVersion());

	QToolBar *toolbar = addToolBar("tb");
	QAction *actExit = toolbar->addAction(QIcon(":/exit"), tr("&Quit"), this, SLOT(quit()));
	actExit->setShortcut(QKeySequence::Quit);
	QAction *actOpen = toolbar->addAction(QIcon(":/fileopen")), tr("Open"), this, SLOT(open());
	actOpen->setShortcut(QKeySequence::Open);

	ToolBarSpacer::addSpacer(toolbar);
	toolbar->addAction(QIcon(":/info"), tr("&About"), this, SLOT(about()));

	statusBar()->addPermanentWidget(m_fileLabel = new QLabel);

	QWidget *cw = new QWidget;
	setCentralWidget(cw);

	QHBoxLayout *lay =new QHBoxLayout(cw);
	lay->addStretch();
	QGroupBox *gb = new QGroupBox("Leds");
	lay->addWidget(gb);
	QFormLayout *form = new QFormLayout(gb);

	foreach (const LedIconSource::LedColor c, LedIconSource::colors())
	{
		form->addRow(QVariant(c).toString(), new LedIcon(c));
		toolbar->addWidget(new LedIcon(c));
	}
	gb = new QGroupBox("Buttons");
	lay->addWidget(gb);
	QVBoxLayout *lay1 = new QVBoxLayout(gb);
	foreach (const LedIconSource::LedColor c, LedIconSource::colors())
	{
		LedButton *b = new LedButton(c, QVariant(c).toString());
		if (c == LedIconSource::On)
		{
			b->setCheckable(true);
			connect(b, &LedButton::toggled, b, &LedButton::setChecked);
		}
		if (c > LedIconSource::On)
		{
			connect(b, &LedButton::clicked, this, &MainWindow::changeLed);
		}
		lay1->addWidget(b);
	}
	lay1->addStretch();
	lay->addStretch();
}

void MainWindow::quit()
{
	close();
}

void MainWindow::open()
{
//	qDebug() << Q_FUNC_INFO;
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
							m_conf->value("/home/dir", qApp->applicationFilePath()).toString(),
							tr("Voltage-Curves (*.psu-set);;All Files (*)"));
	if (! fileName.isEmpty())
	{
		m_fileLabel->setText(filename);
	}
}

void MainWindow::about()
{
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
		.arg(QString(__DATE__).section(' ', -1, -1))
		.arg(qApp->organizationName())
		.arg(qApp->organizationDomain())
		.arg(qVersion())
		;
	QMessageBox::about(this, qApp->applicationName(), text);
}

void MainWindow::changeLed()
{
	LedButton *b = qobject_cast<LedButton*>(sender());
	if (b)
	{
		qDebug() << Q_FUNC_INFO << b << b->led();
		LedIconSource::setOnColor(b->led());
	}
}
