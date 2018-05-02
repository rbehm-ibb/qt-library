// ******************************************************
// * copyright (C) 2016 by Reinhardt Bhm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class CsvTableModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private slots:
	void quit();
	void about();
	void open();
	void append();
	void nameChanged(const QString name);
	void error(const QString msg);
private:
	QLabel *m_fileLabel;
	CsvTableModel *m_model;
};

#endif // MAINWINDOW_H
