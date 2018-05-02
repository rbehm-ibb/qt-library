// ******************************************************
// * copyright (C) 2014 by Reinhardt Behm/EAE rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "project.h"

class OsmView;
class ImportView;
class CalibWidget;
class CoordWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void exit();
	void changedZoom(int);
	void about();
	void goHome();
	void mapHostActive(bool);
	void open();
	void mousePosOsmSlot(QPointF p);
	void mousePosPixSlot(QPointF p);
	void showCalib(QTransform trans, QPolygonF geo, QPointF tl);
	void trans();
	void makeQmp();
private:
	QImage image;
	QPixmap pixmap;
	OsmView * osmView;
	ImportView * impView;
	QAction * actZoomP;
	QAction * actZoomM;
	QSlider * zoomSlider;
	QAction * actExit;
	QAction * actAbout;
	QAction * actHome;
	QLabel * mapHost;
	QLabel * pictname;
	CoordWidget * mousePosPix;
	CoordWidget * mousePosOsm;
	CalibWidget * calib;
	CoordWidget * pos0;
	CoordWidget * size;
	CoordWidget * sized;
	QGraphicsPixmapItem *pixOnMap;
	QGraphicsPolygonItem *polyOnMap;
	QTransform m_trans;
	QRect m_pixRect;
	QRectF m_geoRect;
};

#endif // MAINWINDOW_H
