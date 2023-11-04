// ******************************************************
// * copyright (C) 2023 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 1/30/2023 by behm
// ******************************************************

#include <cstring>
#include "bytevector.h"

ByteVector::ByteVector()
{

}

ByteVector::ByteVector(ConstIterator first, ConstIterator last)
	: QVector<uint8_t>(first, last)
{
}

ByteVector::ByteVector(const uint8_t *d, size_t n)
	: QVector<uint8_t>(n)
{
	for (size_t i = 0; i < n; ++i)
	{
		(*this)[i] = d[i];
	}
}

ByteVector::ByteVector(const uint8_t d, size_t n)
	: QVector<uint8_t>(n)
{
	memset(data(), d, n);
}

void ByteVector::fill(const uint8_t d)
{
	memset(data(), d, size());
}

int ByteVector::dataCopy(uint8_t *d) const
{
	memcpy(d, QVector<uint8_t>::data(), size());
	return size();
}

void ByteVector::expandTo(int n)
{
	while (size() < n)
	{
		append(0);
//		int i =  size();
//		resize(n);
//		for ( ; i < n; ++i)
//		{
//			(*this)[i] = 0;
//		}
	}
	while (size() > n)
	{
		removeLast();
	}
}

QByteArray ByteVector::toBA() const
{
	const uint8_t *d = data();
	const char *dd = (const char *)(d);
	QByteArray ba(dd, size());
	return ba;
}

QString ByteVector::toHex(char sep) const
{
	return QString(toBA().toHex(sep));
}

void ByteVector::fromBa(const QByteArray &d)
{
	clear();
	reserve(d.size());
	for (const char &c : d)
	{
		append(uint8_t(c));
	}
}

ByteVector ByteVector::mid(int first, int len) const
{
	if (len == -1)
	{
		len = size() - first;
	}
	assert(first >= 0);
	assert(len >= 0);
	assert(first+len <= size());
	ByteVector res;
	res.reserve(len);
	while(len > 0)
	{
		res.append(value(first++));
		--len;
	}
	return res;
}

ByteVector ByteVector::last(int len) const
{
	assert(len >= 0);
	assert(len <= size());
	ByteVector res;
	res.reserve(len);
	for  (int i  = size()-len; len > 0; ++i, --len)
	{
		res.append(value(i));
	}
	return res;
}

ByteVector &ByteVector::operator^=(uint32_t n)
{
	assert(size() == 4);
	(*this)[0] ^= ((n >> 24) & 0xff);
	(*this)[1] ^= ((n >> 16) & 0xff);
	(*this)[2] ^= ((n >> 8) & 0xff);
	(*this)[3] ^= ((n >> 0) & 0xff);
	return *this;
}

ByteVector operator^(const ByteVector &v1, const ByteVector &v2)
{
	assert(v1.size() == v2.size());
	ByteVector rc(v1);
	for (int i =  0; i < rc.size(); ++i)
	{
		rc[i] ^= v2[i];
	}
	return rc;
}

ByteVector operator|(const ByteVector &v1, const ByteVector &v2)
{
	assert(v1.size() == v2.size());
	ByteVector rc(v1);
	for (int i =  0; i < rc.size(); ++i)
	{
		rc[i] |= v2[i];
	}
	return rc;
}

ByteVector operator&(const ByteVector &v1, const ByteVector &v2)
{
	assert(v1.size() == v2.size());
	ByteVector rc(v1);
	for (int i =  0; i < rc.size(); ++i)
	{
		rc[i] &= v2[i];
	}
	return rc;
}

ByteVector operator+(const ByteVector &v1, const ByteVector &v2)
{
	ByteVector rc(v1);
	rc.append(v2);
	return rc;
}

ByteVector operator^(const ByteVector &v1, const uint64_t n)
{
	assert(v1.size() == sizeof(n));
	ByteVector res(v1);
	res[0] ^= ((n >> 7*8) & 0xff);
	res[1] ^= ((n >> 6*8) & 0xff);
	res[2] ^= ((n >> 5*8) & 0xff);
	res[3] ^= ((n >> 4*8) & 0xff);
	res[4] ^= ((n >> 3*8) & 0xff);
	res[5] ^= ((n >> 2*8) & 0xff);
	res[6] ^= ((n >> 1*8) & 0xff);
	res[7] ^= ((n >> 0*8) & 0xff);
	return res;
}
