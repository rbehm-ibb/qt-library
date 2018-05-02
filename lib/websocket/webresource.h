// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/20/2017 by behm
// ******************************************************

#ifndef WEBRESOURCE_H
#define WEBRESOURCE_H

class WebSocket;

class WebResource : public QObject
{
	Q_OBJECT
public:
	enum MessageType { FullUpdate = 0 };
	Q_ENUM(MessageType)
	explicit WebResource(QString name, QObject *parent = 0);
	virtual ~WebResource();
	virtual const QByteArray dataToSend() const = 0;
	virtual void setRxdData(quint16 type, const QByteArray &data, WebSocket *src) = 0;
	virtual void update(quint16 type = FullUpdate) { emit sendData(type, dataToSend(), nullptr); }
signals:
	void dataChanged();
	void sendData(quint16 type, const QByteArray &data, WebSocket *dst);
public slots:
//	virtual void setData(const QByteArray &data) = 0;
private slots:
protected:
private:
//	QByteArray m_data;
};

#endif // WEBRESOURCE_H
