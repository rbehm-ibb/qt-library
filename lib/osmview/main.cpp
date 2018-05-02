// ******************************************************
// * copyright (C) 2014 by Becker Electronics Taiwan Ltd.
// * All Rights reserved
// * File: main.cpp
// * created 12/9/2014 by behm
// ******************************************************

#include "project.h"
#include "osmwidget.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	OsmWidget osm;
	osm.setPath("/map/osm/");
	osm.show();
	return app.exec();
}
