// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/30/2023 by behm
// ******************************************************

#ifndef BYTEVECTOR_H
#define BYTEVECTOR_H

class ByteVector : public QVector<uint8_t>
{
	friend ByteVector operator+(const ByteVector &v1, const ByteVector &v2);	// cat
	friend ByteVector operator^(const ByteVector &v1, const ByteVector &v2);
	friend ByteVector operator|(const ByteVector &v1, const ByteVector &v2);
	friend ByteVector operator&(const ByteVector &v1, const ByteVector &v2);
	friend ByteVector operator^(const ByteVector &v1, const uint64_t n);
public:
	ByteVector();
	ByteVector(size_t n): QVector<uint8_t>(n) {}
	ByteVector(ByteVector::ConstIterator first, ByteVector::ConstIterator last);
	ByteVector(const uint8_t*d, size_t n);
	ByteVector(const uint8_t d, size_t n);
	ByteVector(const QByteArray ba) { fromBa(ba); }
	void fill(const uint8_t d);
	int dataCopy(uint8_t *d) const;
	void expandTo(int n);
	QByteArray toBA() const;
	QString toHex(char sep) const;
	void fromBa(const QByteArray &d);
	ByteVector mid(int first, int  len = -1) const;
	ByteVector last(int  len) const;
	ByteVector &operator^=(uint32_t n);
};

#endif // BYTEVECTOR_H
