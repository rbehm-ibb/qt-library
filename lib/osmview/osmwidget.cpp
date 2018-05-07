// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * File: osmwidget.cpp    OsmWidget
// * created 12/9/2014 by behm
// ******************************************************

#include "osmwidget.h"
#include "osmview.h"
#include "osmtile.h"
//#include "mapscalewidget.h"

OsmWidget::OsmWidget(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(m_toolbar = new QToolBar);
	layout->addWidget(m_osmView = new OsmView);

	m_pos = new QLabel;
	m_actZoomM = new QAction(QIcon(":/zoomout.png"), tr("Zoom-"), this);
	m_actZoomM = m_toolbar->addAction(QIcon(":/zoomout.png"), tr("Zoom-"));
	connect(m_actZoomM, &QAction::triggered, m_osmView, &OsmView::decZoom);
	m_zoomSlider = new QSlider(Qt::Horizontal, this);
	m_zoomSlider->setRange(0, 18);
	m_zoomSlider->setTickPosition(QSlider::TicksBelow);
	m_zoomSlider->setTickInterval(1);
	connect(m_zoomSlider, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), m_osmView, &OsmView::setZoom);
	m_toolbar->addWidget(m_zoomSlider);
	m_actZoomP = m_toolbar->addAction(QIcon(":/zoomin.png"), tr("Zoom+"));
	connect(m_actZoomP, &QAction::triggered, m_osmView, &OsmView::incZoom);

	m_mapHost = new QLabel(tr("Map-Server"));
	m_mapHost->setPixmap(QPixmap(":/network_traffic.png"));
	connect(m_osmView, &OsmView::changedZoom, this, &OsmWidget::changedZoom);
	connect(m_osmView, &OsmView::changedTransform, this, &OsmWidget::changedTransform);
	connect(m_osmView, &OsmView::centerChanged, this, &OsmWidget::centerChanged);
	connect(m_osmView, &OsmView::hostActive, this, &OsmWidget::mapHostActive);
	connect(m_osmView, &OsmView::mousePos, this, &OsmWidget::mousePosOsmSlot);
//	m_scale = new MapScaleWidget(m_osmView);
//	m_scale->move(0, 0);
//	connect(m_osmView, &OsmView::changedTransform, m_scale, &MapScaleWidget::changedScale);
//	m_osmView->setZoom(5);
//	setHome(QPointF(-8.818392, 17.49625));
}

//QGraphicsScene *OsmWidget::scene()
//{
//	return osmView->scene();
//}

QList<QGraphicsItem *> OsmWidget::items() const
{
	return m_osmView->items();
}

void OsmWidget::addItem(QGraphicsItem *item)
{
	m_osmView->addItem(item);
}

void OsmWidget::setPath(QString path)
{
	OsmTile::setPath(path);
}

void OsmWidget::setFetchTiles(bool fetchTiles)
{
	OsmTile::setFetchTiles(fetchTiles);
}

QString OsmWidget::copyright() const
{
	return "<qt>Maps &copy; OpenStreetMap contributor, ODbl</qt> @" + OsmTile::getTileserver();
}

QPointF OsmWidget::center() const
{
	return m_osmView->center();
}

void OsmWidget::ensureVisible(QPointF pos)
{
	m_osmView->ensureVisible(pos);
}

void OsmWidget::unselectAll()
{
	m_osmView->unselectAll();
}

QNetworkAccessManager *OsmWidget::networkManager()
{
	return OsmTile::networkManager();
}

void OsmWidget::setTileserver(const QString tileServer)
{
	OsmTile::setTileserver(tileServer);
}

QString OsmWidget::tileserver() const
{
	return OsmTile::getTileserver();
}

int OsmWidget::zoom() const
{
	return m_osmView->zoom();
}

void OsmWidget::setHome(QPointF p)
{
	m_homePos = p;
	goHome();
}

void OsmWidget::setCenter(QPointF p)
{
	m_osmView->setCenter(p);
}

void OsmWidget::setZoom(int z)
{
	m_osmView->setZoom(z);
}

void OsmWidget::changedZoom(int z)
{
	m_actZoomP->setEnabled(z < 18);
	m_actZoomM->setEnabled(z > 0);
	m_zoomSlider->setValue(z);
}

void OsmWidget::goHome()
{
	m_osmView->setCenter(m_homePos);
	m_osmView->centerOn(m_homePos);
}

void OsmWidget::mapHostActive(bool on)
{
//	qDebug() << Q_FUNC_INFO << on;
	m_mapHost->setPixmap(QPixmap(on ? ":/network_incoming.png" : ":/network_traffic.png"));
}

void OsmWidget::mousePosOsmSlot(QPointF p)
{
	static const QChar symbol(0x00B0);   // degrees symbol

	m_pos->setText(QString::fromLatin1("Pos: %1%2, %3%4").arg(p.x()).arg(symbol).arg(p.y()).arg(symbol));
}





