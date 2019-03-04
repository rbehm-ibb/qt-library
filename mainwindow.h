// ******************************************************
// * copyright (C) 2016 by Reinhardt Bhm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QString filename);

signals:

public slots:

private slots:
	void quit();
	void about();
	void changeLed();
	void open();
private:
	QLabel *m_fileLabel;
};

#endif // MAINWINDOW_H
