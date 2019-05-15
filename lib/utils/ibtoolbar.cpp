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
	, m_what(nullptr)
	, m_noWhat(false)
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

void IBToolBar::removeWhatis()
{
	if (m_what)
	{
		delete m_what;
		m_what = 0;
	}
	m_noWhat = true;
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
	if (! m_noWhat)
	{
		m_what = QWhatsThis::createAction(this);
		addAction(m_what);
		m_what->setStatusTip(tr("Whats this?"));
	}
	m_about = addAction(QIcon(":/toolbarspacer/info.svgz"), tr("&About"));
//	m_about = addAction(QIcon(":/qt-project.org/styles/commonstyle/images/fileinfo-32.png"), tr("&About"));
	m_about->setToolTip(tr("Information about this program"));
	m_about->setWhatsThis(tr("Information about this program"));
	m_about->setStatusTip(tr("About this program"));
	QObject::connect(m_about, &QAction::triggered, this, &IBToolBar::aboutSlot);
	QMainWindow *mw = qobject_cast<QMainWindow *>(parentWidget());
	if (mw)
	{
		mw->setWindowTitle(QString("%1 %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
	}
}

void IBToolBar::addQuit()
{
	QMainWindow *mw = qobject_cast<QMainWindow *>(parentWidget());
	if (mw)
	{
		m_quit = addAction(QIcon(":/toolbarspacer/exit.svgz"), tr("Exit"), mw, SLOT(quit()));
		m_quit->setShortcut(QKeySequence::Quit);
		m_quit->setObjectName("Quit-Action");
		m_quit->setToolTip(m_quit->shortcut().toString() + " " + tr("Leave this program"));
		m_quit->setWhatsThis(tr("Leave this program") + "\n" + m_quit->shortcut().toString());
		m_quit->setStatusTip(m_quit->whatsThis());
	}
	else
	{
		qWarning() << Q_FUNC_INFO << "no QMainWindow for" << this;
	}
}


void IBToolBar::aboutSlot()
{
	QVariant copyr = qApp->property("copyright-icon");
	QString copyIcon;
	if (copyr.isValid())
	{
		copyIcon =  "<img height=20 src=\"" + copyr.toString() + "\">";
	}
	QString text("<h1>%1</h1>"
		     "<p>Version %2"
		     "<p>&copy; %3, %7 %4"
		     "<p>Web: <a href=\"http://%5\">http://%5</a>"
		     "<p>Mail: <a href=\"mailto:info@%5\">info@%5</a>"
		     "<p>Using  <a href=\"http://qt.io\"><img src=\":/stdicons/qt-logo-about.png\"> %6</a>"
		    );
	text = text
	       .arg(qApp->applicationName())
	       .arg(qApp->applicationVersion())
	       .arg(QString(__DATE__).section(' ', -1, -1))
	       .arg(qApp->organizationName())
	       .arg(qApp->organizationDomain())
	       .arg(qVersion())
	       .arg(copyIcon)
	       ;
	QMessageBox::about(parentWidget(), qApp->applicationName(), text);
}
