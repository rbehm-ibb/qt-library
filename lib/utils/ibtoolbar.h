// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/26/2018 by behm
// ******************************************************

#ifndef IBTOOLBAR_H
#define IBTOOLBAR_H

class IBToolBar : public QToolBar
{
	Q_OBJECT
public:
	explicit IBToolBar(QWidget *parent = 0);
	~IBToolBar();
	QAction *quit() const { return m_quit; }
	QAction *about();
	// QObject interface
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

	// QObject interface
	void addAbout();
	void addQuit();
};

#endif // IBTOOLBAR_H
