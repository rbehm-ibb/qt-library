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
	Q_PROPERTY(bool m_maximzeV READ maximzeV WRITE setMaximzeV)
	Q_PROPERTY(bool m_whatis READ whatis WRITE setWhatis)
public:
	explicit IBToolBar(QWidget *parent = nullptr);
	~IBToolBar();
	QAction *quit() const { return m_quit; }
	QAction *about();
	virtual bool event(QEvent *e);

	void setMaximzeV(bool newNoMaximzeV) { m_maximzeV = newNoMaximzeV; }
	bool maximzeV() const { return m_maximzeV; }

	bool whatis() const { return m_whatis; }
	void setWhatis(bool newWhatis) { m_whatis = newWhatis; }

signals:

public slots:
private slots:
	void aboutSlot();
protected:
private:
	QAction *m_quit;
	QAction *m_about;
	bool m_whatis;
	bool m_maximzeV;

	void addAbout();
	void addQuit();
};

#endif // IBTOOLBAR_H
