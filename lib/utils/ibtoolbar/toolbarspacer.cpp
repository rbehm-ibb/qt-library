// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/6/2016 by behm
// ******************************************************

#include "toolbarspacer.h"

ToolBarSpacer::ToolBarSpacer(int width)
	: QWidget(0)
{
	if (width > 0)
	{
		setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		setFixedWidth(width);
	}
	else
	{
		setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	}
}

void ToolBarSpacer::addSpacer(QToolBar *tb, int width)
{
	tb->addWidget(new ToolBarSpacer(width));
}

QAction *ToolBarSpacer::addAbout(QMainWindow *mw)
{
	Q_ASSERT(mw != 0);
	QToolBar *tb = mw->findChild<QToolBar*>();
	if (tb)
	{
		ToolBarSpacer *tbs = new ToolBarSpacer(0);
		tb->addWidget(tbs);
		ToolBarSpacer::addSpacer(tb);
		QAction *act = tb->addAction(QIcon(":/toolbarspacer/info.svgz"), tr("&About"));
		QObject::connect(act, &QAction::triggered, tbs, &ToolBarSpacer::about);
		return act;
	}
	else
	{
		qWarning() << Q_FUNC_INFO << "no QToolBar for" << mw;
		return nullptr;
	}
}

QAction *ToolBarSpacer::addAbout(QToolBar *tb, QWidget *parent, const char aboutSlot[])
{
	ToolBarSpacer::addSpacer(tb);
	return tb->addAction(QIcon(":/toolbarspacer/exit.svgz"), tr("About"), parent, aboutSlot);
}

QAction *ToolBarSpacer::addQuit(QMainWindow *mw)
{
	Q_ASSERT(mw != 0);
	QToolBar *tb = mw->findChild<QToolBar*>();
	if (tb)
	{
		// TODO: how can this action be made this first in the toolbar?
		QAction *act = tb->addAction(QIcon(":/toolbarspacer/exit.svgz"), tr("Exit"), mw, SLOT(quit()));
		act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
		act->setObjectName("Quit-Action");
		return act;
	}
	else
	{
		qWarning() << Q_FUNC_INFO << "no QToolBar for" << mw;
		return nullptr;
	}
}


void ToolBarSpacer::about()
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
	QMessageBox::about(this->parentWidget(), qApp->applicationName(), text);
}
