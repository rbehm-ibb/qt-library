// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/2/2023 by behm
// ******************************************************

#ifndef MAXIMIZEV_H
#define MAXIMIZEV_H

#include <QAction>
#include <QObject>

class MaximizeV : public QAction
{
	Q_OBJECT
public:
	explicit MaximizeV(QObject *parent = nullptr);
private slots:
	void trigged();
};

#endif // MAXIMIZEV_H
