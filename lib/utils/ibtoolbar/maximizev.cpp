// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/2/2023 by behm
// ******************************************************

#include "maximizev.h"

MaximizeV::MaximizeV(QObject *parent)
	: QAction{parent}
{
	setObjectName("maximizeV");
	setIcon(QIcon(":/toolbarspacer/top.svgz"));
	setStatusTip(tr("Maximize vertically"));
	connect(this, &QAction::triggered, this, &MaximizeV::trigged);
}

void MaximizeV::trigged()
{
//	qDebug() << Q_FUNC_INFO << this << this->parentWidget();
	QMainWindow *mw = qobject_cast<QMainWindow*>(this->parent());
	if (mw)
	{
		QScreen *dt = qApp->primaryScreen();
		QSize sz = dt->availableGeometry().size();
		QSize msz = mw->size();
//		qDebug() << Q_FUNC_INFO << dt->screenNumber(mw) << dt->screenGeometry(mw) << dt->availableGeometry(mw) << sz;
		msz.setHeight(sz.height());
		mw->resize(msz);
	}
//	setDisabled(true);
}
