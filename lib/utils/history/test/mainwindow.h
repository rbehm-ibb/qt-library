// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 10/26/2018 by behm
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class FileHistory;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void quit();
private slots:
	void historySelected(const QString &s);
	void on_actionOpen_triggered();

private:

	void openFile(const QString &fn);
	Ui::MainWindow *ui;
	FileHistory *m_hist;
};

#endif // MAINWINDOW_H
