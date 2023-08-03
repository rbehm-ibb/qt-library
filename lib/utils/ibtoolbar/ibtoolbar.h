// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/26/2018 by behm
// ******************************************************

#ifndef IBTOOLBAR_H
#define IBTOOLBAR_H

#include <QToolBar>

class IBToolBar : public QToolBar
{
	Q_OBJECT
public:
	explicit IBToolBar(QWidget *parent = nullptr);
	~IBToolBar();
	QAction *quit() const { return m_quit; }
	QAction *about();
	void removeWhatis();
	void removeMaximizeV();
public:
	virtual bool event(QEvent *e);

signals:

public slots:
private slots:
	void aboutSlot();
protected:
private:
	QAction *m_quit;
	QAction *m_about;
	QAction *m_what;
	QAction *m_maximizeV;
	bool m_noWhat;

	void addAbout();
	void addQuit();
};

#endif // IBTOOLBAR_H
