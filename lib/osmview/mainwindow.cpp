// ******************************************************
// * copyright (C) 2014 by Reinhardt Behm/EAE rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#include "project.h"
#include "setting.h"
#include "mainwindow.h"
#include "osmview.h"
#include "importview.h"
#include "coordwidget.h"
#include "qmpfile.h"
#include "calibwidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, pixOnMap(0)
	, polyOnMap(0)
{
	setWindowIcon(QIcon(":/icons/target.png"));
	setWindowTitle(qApp->applicationName() + " " +qApp->applicationVersion());

	QLabel * copyright = new QLabel("<qt>Maps &copy; OpenStreetMap, CC-BY-SA</qt>");
	statusBar()->addPermanentWidget(copyright);
	mapHost = new QLabel(tr("Map-Server"));
	mapHost->setPixmap(QPixmap(":/icons/network_traffic.png"));
	statusBar()->addPermanentWidget(mapHost);
	statusBar()->addWidget(pictname = new QLabel);
	statusBar()->addWidget(mousePosOsm = new CoordWidget(2));
	statusBar()->addWidget(mousePosPix = new CoordWidget(0));

	QSplitter *mw = new QSplitter(this);
	osmView = new OsmView(this);
	setCentralWidget(mw);
	connect(osmView, SIGNAL(changedZoom(int)), this, SLOT(changedZoom(int)));
	connect(osmView, SIGNAL(hostActive(bool)), this, SLOT(mapHostActive(bool)));
	mw->addWidget(osmView);

	calib = new CalibWidget(this);;
	calib->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	mw->addWidget(calib);
	impView = new ImportView(this);
	mw->addWidget(impView);

	QToolBar *toolbar = addToolBar("load");
	actExit = toolbar->addAction(QIcon(":/icons/exit.png"), tr("&Quit"), this, SLOT(exit()));
	actExit->setShortcut(QKeySequence(tr("Ctrl+Q", "File|Quit")));
	toolbar->addAction(QIcon("://icons_famfam/scalable/actions/fileopen.svgz"), tr("&Open"), this, SLOT(open()));

	toolbar->addSeparator();
	toolbar->addAction(QIcon("://icons_famfam/scalable/actions/rotate_ccw.svgz"), "Rotate", impView, SLOT(rotate()));
	toolbar->addAction("Transform", this, SLOT(trans()));
	toolbar->addAction("QMP", this, SLOT(makeQmp()));
	actHome = toolbar->addAction(QIcon(":/icons/home.png"), tr("&Home"), this, SLOT(goHome()));
	actHome->setShortcut(QKeySequence(tr("Alt+H")));
	actHome->setWhatsThis(tr("Center the map at the configured home position"));

	QToolBar *zoombar = addToolBar("Zoom");
	actZoomM = new QAction(QIcon(":/icons/zoomout.png"), tr("Zoom-"), this);
	actZoomM = zoombar->addAction(QIcon(":/icons/zoomout.png"), tr("Zoom-"), osmView, SLOT(decZoom()));
	zoomSlider = new QSlider(Qt::Horizontal, this);
	zoomSlider->setRange(4, 18);
	zoomSlider->setTickPosition(QSlider::TicksBelow);
	zoomSlider->setTickInterval(1);
	connect(zoomSlider, SIGNAL(valueChanged(int)), osmView, SLOT(setZoom(int)));
	zoombar->addWidget(zoomSlider);
	actZoomP = zoombar->addAction(QIcon(":/icons/zoomin.png"), tr("Zoom+"), osmView, SLOT(incZoom()));

	toolbar->addSeparator();
	actAbout = toolbar->addAction(QIcon(":/icons/help.png"), tr("&About"), this, SLOT(about()));

	osmView->setZoom(5);

	connect(osmView, &OsmView::mousePos, this, &MainWindow::mousePosOsmSlot);
	connect(impView, &ImportView::mousePos, this, &MainWindow::mousePosPixSlot);
	connect(impView, &ImportView::clickedPos, calib, &CalibWidget::setPix);

	connect(calib, &CalibWidget::showCalib, this, &MainWindow::showCalib);
	connect(calib, &CalibWidget::calibChangedPix, impView, &ImportView::calibChangedPix);
}

MainWindow::~MainWindow()
{
}

void MainWindow::exit()
{
	close();
}

void MainWindow::changedZoom(int z)
{
	actZoomP->setEnabled(z < 18);
	actZoomM->setEnabled(z > 0);
	zoomSlider->setValue(z);
}

void MainWindow::about()
{
	QMessageBox::about(this, qApp->applicationName(),
			"<h1>" + qApp->applicationName() + "</h1>"
			"<p>Version " + qApp->applicationVersion() +
			"<p>&copy; 2014, " + qApp->organizationName()
			);
}

void MainWindow::goHome()
{
	osmView->setCenter(QPointF(conf.value("/start/pos", QPoint(-8818392, 1749625)).toPoint()) * 1e-5);
	osmView->centerOn(QPointF(conf.value("/start/pos", QPoint(-8818392, 1749625)).toPoint()) * 1e-5);
}

void MainWindow::mapHostActive(bool on)
{
	mapHost->setPixmap(QPixmap(on ? ":/icons/network_incoming.png" : ":/icons/network_traffic.png"));
}

void MainWindow::open()
{
	const static QString confTag("/pictfile");
	QString fname = QFileDialog::getOpenFileName(this,
						     "Open Picture",
						     conf.value(confTag, "./").toString(),
						     "Images (*.png *.xpm *.jpg *.jpeg *.tif *.tiff *.bmp *.pcx)"
						     );
	if (! fname.isEmpty())
	{
		image = QImage();
		pixmap = QPixmap();
		impView->setPixmap(pixmap);
		if (pixOnMap)
			delete pixOnMap;
		pixOnMap = 0;
		if (image.load(fname))
		{
			conf.setValue(confTag, fname);
			pictname->setText(fname.section("/", -1));
			pixmap = QPixmap::fromImage(image);
			impView->setPixmap(pixmap);
			qDebug() << Q_FUNC_INFO << "scene" << image.size();
			calib->setName(pictname->text());
			calib->load();
		}
		else
			qDebug() << Q_FUNC_INFO << fname << "no load";
	}
}

void MainWindow::mousePosOsmSlot(QPointF p)
{
	mousePosOsm->setValue(p);
}

void MainWindow::mousePosPixSlot(QPointF p)
{
	mousePosPix->setValue(p);
}

void MainWindow::showCalib(QTransform trans, QPolygonF geo, QPointF tl)
{
	m_trans = trans;
	if (pixOnMap)
		delete pixOnMap;
	pixOnMap = osmView->scene()->addPixmap(pixmap);
	pixOnMap->setZValue(10);
	pixOnMap->setOpacity(0.3);
	pixOnMap->setTransform(trans);
	pixOnMap->setPos(tl);
	if (polyOnMap)
		delete polyOnMap;
	polyOnMap = osmView->scene()->addPolygon(geo);
	polyOnMap->setPos(tl);
	polyOnMap->setPen(QPen(Qt::red, 0));
	polyOnMap->setBrush(Qt::NoBrush);
	polyOnMap->setZValue(20);
	qDebug() << Q_FUNC_INFO << m_trans << pixOnMap->boundingRect();
}

void MainWindow::trans()
{
	QPointF geoTl = calib->geoTopleft();
	QPolygonF geo = calib->geo();
	const qreal scaleY = 1500.;
	const qreal scaleX = scaleY;
	for (int i = 0; i < 4; ++i)
	{
		geo[i].rx() *= scaleX;
		geo[i].ry() *= -scaleY;
	}
	QPolygonF pix = calib->pix();

	QTransform transform;
	QTransform::quadToQuad(pix, geo, transform);

	QTransform trueTransform = image.trueMatrix(transform, image.width(), image.height());
//	qDebug() << trueTransform << endl << trueTransform.mapRect(image.rect()) << endl << trueTransform.mapToPolygon(image.rect());

	QImage it = image.transformed(transform, Qt::SmoothTransformation);

	m_geoRect = calib->geo().translated(geoTl).boundingRect();
	m_pixRect = trueTransform.map(calib->pix()).boundingRect().toRect();

	qreal dy = m_geoRect.height()/m_pixRect.height();
	int offsetY = -50;
	m_pixRect.translate(0, offsetY);
	m_geoRect.translate(0, -offsetY*dy/2);

	qDebug() << "in" << pix.boundingRect().size() << "out" << m_pixRect.size();
	qDebug() << "res" << it.size() << m_pixRect << m_geoRect;

	image = it.copy(m_pixRect);
	qDebug() << "res" << image.size();
}

void MainWindow::makeQmp()
{
	const static QString confTag("/qmpfile");
	QFileInfo fi( conf.value(confTag, "./").toString());
	QString ffile = fi.dir().absoluteFilePath(calib->name() + ".qmp");
	QString fname = QFileDialog::getSaveFileName(this,
						     "Save as QMP",
						     ffile,
						     "QMP (*.qmp)"
						     );
	if (! fname.isEmpty())
	{
		if (! fname.endsWith(".qmp"))
			fname.append(".qmp");
		conf.setValue(confTag, fname);
		QmpFile qmp;
//		QRectF r = calib->geo().boundingRect();
//		QPointF tl = r.topLeft() + calib->geoTopleft();
		qDebug() << m_geoRect/* << tl*/;
		QSizeF sz(m_geoRect.size());
		qmp.setRect(m_geoRect);
		qmp.setDate(calib->date());
		qmp.setCopyright(calib->copyright());
		qmp.setPix(image);
		qmp.create(fname);
	}
}
