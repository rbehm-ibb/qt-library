// ******************************************************
// * copyright (C) 2019 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/15/2019 by behm
// ******************************************************

#ifndef SERIALPORTSELECTOR_H
#define SERIALPORTSELECTOR_H


class SerialPortSelector : public QComboBox
{
	Q_OBJECT
public:
	explicit SerialPortSelector(QWidget *parent = nullptr);
	~SerialPortSelector();
	void init(const QString port);
	QString portname() const;
signals:
	void portChanged(const QString portname);
public slots:
	void on_currentIndexChanged(int index);
	void refresh();
private slots:
protected:
private:
};

#endif // SERIALPORTSELECTOR_H
