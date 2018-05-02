// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/22/2016 by behm
// ******************************************************

#ifndef FILESELECTOR_H
#define FILESELECTOR_H


class FileSelector : public QWidget
{
	Q_OBJECT
public:
	explicit FileSelector(QWidget *parent = 0);
	QString fileName() const { return m_name->text(); }
signals:
	void changed(const QString fn);
	void sigView(const QString fn);
public slots:
	void setFn(const QString fn);
	void setDir(const QString dir);
private slots:
	void selectFile();
	void viewFile();
protected:
private:
	QLineEdit *m_name;
	QPushButton *m_selButton;
	QPushButton *m_viewButton;
	QString m_homeDir;
};

#endif // FILESELECTOR_H
