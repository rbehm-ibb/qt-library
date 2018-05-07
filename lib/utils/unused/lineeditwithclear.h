// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/9/2017 by behm
// ******************************************************

#ifndef LINEEDITWITHCLEAR_H
#define LINEEDITWITHCLEAR_H


class LineEditWithClear : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged USER true)
public:
	explicit LineEditWithClear(QWidget *parent = 0);
//	~LineEditWithClear();
	QString text() const { return m_line->text(); }
	QLineEdit *line() const { return m_line; }
	void setReadOnly(bool ro);
	void addButton(QPushButton *b);
	void addButton(QAction *a);
signals:
	void textChanged(const QString &);
	void returnPressed();
public slots:
	void setText(const QString txt) { m_line->setText(txt); }
private slots:
//	void clear();
protected:
private:
	QHBoxLayout *m_layout;
	QLineEdit *m_line;
	QPushButton *m_clearButton;
};

#endif // LINEEDITWITHCLEAR_H
