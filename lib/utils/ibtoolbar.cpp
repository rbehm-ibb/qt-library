// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/26/2018 by behm
// ******************************************************

#include "ibtoolbar.h"

IBToolBar::IBToolBar(QWidget *parent)
	: QToolBar(parent)
	, m_quit(nullptr)
	, m_about(nullptr)
{
	addQuit();
}

IBToolBar::~IBToolBar()
{
}

QAction *IBToolBar::about()
{
	if (! m_about)
	{
		addAbout();
	}
	return m_about;
}

bool IBToolBar::event(QEvent *e)
{
//	qDebug() << Q_FUNC_INFO << e;
	if (e->type() == QEvent::ChildPolished)
	{
		if (! m_about)
		{
			addAbout();
		}
	}
	return QToolBar::event(e);
}

void IBToolBar::addAbout()
{
	QWidget *tbs = new QWidget;
	tbs->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	addWidget(tbs);
	m_about = addAction(QIcon(":/toolbarspacer/info.svgz"), tr("&About"));
	QObject::connect(m_about, &QAction::triggered, this, &IBToolBar::aboutSlot);
}

void IBToolBar::addQuit()
{
	QMainWindow *mw = qobject_cast<QMainWindow*>(parentWidget());
	if (mw)
	{
		// TODO: how can this action be made this first in the toolbar?
		m_quit = addAction(QIcon(":/toolbarspacer/exit.svgz"), tr("Exit"), mw, SLOT(quit()));
		m_quit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
		m_quit->setObjectName("Quit-Action");
	}
	else
	{
		qWarning() << Q_FUNC_INFO << "no QMainWindow for" << this;
	}
}


void IBToolBar::aboutSlot()
{
	QString text("<h1>%1</h1>"
		     "<p>Version %2"
		     "<p>&copy; %3, %4"
		     "<p>Web: <a href=\"http://%5\">http://%5</a>"
		     "<p>Mail: <a href=\"mailto:info@%5\">info@%5</a>"
		     "<p>Using  <img src=\":/toolbarspacer/qt-logo-about.png\"> %6"
		     );
	text = text
		.arg(qApp->applicationName())
		.arg(qApp->applicationVersion())
		.arg(QString(__DATE__).section(' ', -1, -1))
		.arg(qApp->organizationName())
		.arg(qApp->organizationDomain())
		.arg(qVersion())
		;
	QMessageBox::about(parentWidget(), qApp->applicationName(), text);
}