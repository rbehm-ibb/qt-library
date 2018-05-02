// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 6/20/2017 by behm
// ******************************************************

#include "webresource.h"
// #include "websocket.h"

WebResource::WebResource(QString name, QObject *parent)
	: QObject(0)
{
	setObjectName(name);
	setParent(parent);
}

WebResource::~WebResource()
{
}
