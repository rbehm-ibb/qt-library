// ******************************************************
// * copyright (C) 2021 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 7/26/2021 by behm
// ******************************************************

#ifndef STREAMLINEFILTER_H
#define STREAMLINEFILTER_H


class StreamLineFilter : public QObject
{
	Q_OBJECT
public:
	explicit StreamLineFilter(QIODevice *dev, QObject *parent = nullptr);
	virtual ~StreamLineFilter() override;
	QByteArray readLine();						///< get a line if avail, empty if non avail
	bool canReadLine() const { return ! m_lines.isEmpty(); }	///< any lines available?
	const QByteArray &bol() const { return m_bol; }
	void setBol(const QByteArray &newBol) { m_bol = newBol; }
	const QByteArray &eol() const { return m_eol; }
	void setEol(const QByteArray &newEol) { m_eol = newEol; }
	const QByteArray &ignore() const { return m_ignore; }
	void setIgnore(const QByteArray &newIgnore) { m_ignore = newIgnore;  }
	void setMaxLineLength(int n) { m_maxLineLength = n; }		///< set the \param n max length of a line accepted
	int maxLineLength() const { return m_maxLineLength; }		///< get the max line length
	void setMaxLine(int n) { m_maxLines = n; }			///< set the \param n max number of lines stored internally
	int maxLines() const { return m_maxLines; }			///< get the max lines
	void setRxTimeout(int msec) { m_rxTimeoutMsec = msec; }		///< set the \param msec byte to byte timeout
	int rxTimeout() const { return m_rxTimeoutMsec; }		///< get the byte to byte timeout

signals:
	void lineRxd(StreamLineFilter *that);
	void rxOverflow();
	void rxTimeout();	///< timeout inside a line
public slots:
	void sendLine(const QByteArray &line);
private slots:
	void readRxdDataSlot();	///< internal, read rxd data fom SerialPort, called when data rxd.
	void rxTimeoutSlot();	///< handle byte to byte timeouts
protected:
	QIODevice * const m_dev;
	QTimer *m_rxTimer;	///<  handle byte to byte timeouts
	QByteArray m_bol;
	QByteArray m_eol;
	QByteArray m_ignore;
	int m_rxTimeoutMsec;	///< the byte to byte timeout in msec
	int m_maxLineLength;	///< max length of a line accepted, default = 1000
	int m_maxLines;		///< max number of line stored internally, default = 10
	bool m_hadBol;		///< did we receive a bol?
	QByteArray m_rxdData;	///< collects the next received line
	QQueue<QByteArray> m_lines;	///< collects already received lines.

};

#endif // STREAMLINEFILTER_H
