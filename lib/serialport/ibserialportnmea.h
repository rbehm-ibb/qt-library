// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 12/26/2015 by behm
// ******************************************************

#ifndef IBSERIALPORTNMEA_H
#define IBSERIALPORTNMEA_H

#include "ibserialportline.h"

/// \brief The IBSerialPortNmea class handles a general NMEA-0183 protocol.
///
/// It receives NMEA sentences via a properly setup IBSerialPortLine (BOL="$", EOL=\\n, IGNORE=\\r).
/// When a line has been rxd, it test for a valid checksum, if there is any.
/// Messages with invalid checksum are discared and signalled with chksumError().
///
/// Valid messages are split into their parts (split at ",") and the signalled with nmeaRxd().
///
/// Sending of messages is achieved with the various send...() functions. they take care of assembling
/// the messages in the correct format and appending a cheksum at the end.
///
/// Some helper function are useful to convert from and to the usual NMEA data formats like coordinates, date and time
/// as used by the NMEA-0183 GPS messages.
///
class IBSerialPortNmea : public IBSerialPortLine
{
	Q_OBJECT
public:
	/// \brief Creates a IBSerialPortNmea
	/// \param device The device to use (ttySx) with optional baudrate appended
	/// \param parent For QObject
	IBSerialPortNmea(const QString device, QObject *parent = 0);
	void send(const QByteArray &a);		///< send the \param a as one NMEA message, prepended with $ and checksum appended
	void sendStart(const QByteArray &a);	///< send the \param a as the start of a NMEA message prepended with $
	void sendPart(const QByteArray &a);	///< send \param a as field of a NMEA message with , prepended
	void sendLastPart(const QByteArray &a);	///< sendPart() + sendEnd(). \param a part to send
	void sendEnd();				///< send checksum and \\r\\n

	/// converts time in NMEA format to QTime.
	/// \param time The time in NMEA format (yymmdd)
	/// \returns Converted time or an invalid QTime on error
	static QTime toTime(const QByteArray &time);
	/// converts packed ddmmyy in NMEA format to QDate
	/// \param d The date in packed NMEA format (ddmmyy)
	/// \returns Converted data or an invalid QDate on error
	static QDate toDate(const QByteArray &d);
	/// converts yy, mm, dd NMEA format to QDate, returns an invalid QDate on error
	/// \param day The NMEA day
	/// \param mon The NMEA month
	/// \param year the NMEA year
	/// \returns Converted data or an invalid QDate on error
	static QDate toDate(const QByteArray &day, const QByteArray &mon, const QByteArray &year);
	/// converts coord + hemisphere in NMEA format to double, return NAN on error.
	/// \param num The coord in NMEA format (dddmm.mmmm, N)
	/// \param hemi The hemisphere (N|S|E|W)
	/// \returns The converted value or NAN on error.
	static double toCoord(const QByteArray &num, const QByteArray &hemi);

	void sendXcoord(qreal x);		///< send a longitude in NMEA format (dddmm.mmm,H)
	void sendYcoord(qreal y);		///< send a latitude in NMEA format (ddmm.mmm,H)
	void sendCoord(const QPointF &p);	///< send a latitude and longitude in NMEA format
	void sendCoord(qreal x, qreal y);	///< send a latitude and longitude in NMEA format
	void sendTime(QTime time);
	qint64 write(const char *data);
signals:
	/// we have received a valid NMEA message.
	/// \param parts the rxd parts (w/o chksum).
	void nmeaRxd(const QList<QByteArray> parts);
	/// The was an invalid checksum
	/// \param line The line (w/o chksum)
	void chksumError(const QByteArray line);
public slots:
private slots:
	void lineRxdSlot();	///< gets signal from base class if a line has been received
private:
	quint8 m_chksum;	///< running checksum when sending parts of a message
	void calcChksum(const QByteArray &a);	///< accumulative calculation of checksum
};

#endif // IBSERIALPORTNMEA_H
