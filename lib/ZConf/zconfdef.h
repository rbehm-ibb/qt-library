// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/13/2022 by behm
// ******************************************************

#ifndef ZCONFDEF_H
#define ZCONFDEF_H


class ZConfDef
{
public:
	ZConfDef() {}
	static QString magic() { return m_magic;}
	static uint16_t udpPort() { return 45454; }
	static uint16_t udpPortreply() { return 45455; }
	static uint16_t tcpPort() { return 45000; }
private:
	static const  QString m_magic;
};

#endif // ZCONFDEF_H
