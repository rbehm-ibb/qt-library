// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 2/22/2017 by behm
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class BarScale;
class ArcScale;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
private slots:
	void chLow(qreal v);
	void chHigh(qreal v);
protected:
private:
	BarScale * vb;
	ArcScale * hb;
};

#endif // MAINWINDOW_H
