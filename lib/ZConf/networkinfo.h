// ******************************************************
// * copyright (C) 2022 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/13/2022 by behm
// ******************************************************

#ifndef NETWORKINFO_H
#define NETWORKINFO_H


class NetworkInfo
{
public:
	NetworkInfo() {}
	static QHostAddress myAddress();
	static QDateTime startTime();
};

#endif // NETWORKINFO_H
