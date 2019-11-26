// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 8/29/2018 by behm
// ******************************************************

#ifndef SINGLEAPP_H
#define SINGLEAPP_H

class SingleApp : public QObject
{
	Q_OBJECT
public:
	explicit SingleApp(bool kill = true, QObject *parent = nullptr);
	~SingleApp();

signals:
	void kill();
public slots:
private slots:
	void newConnection();
	void readyReadSlot();
	void disconnectedSlot();
protected:
private:
	QPointer<QLocalServer> m_server;
	QPointer<QLocalSocket> m_socket;
	QString m_sockName;
};

#endif // SINGLEAPP_H
