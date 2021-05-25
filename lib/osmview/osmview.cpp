// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#include "osmview.h"
#include "osmscene.h"

OsmView::OsmView(QWidget *parent)
	: QGraphicsView(parent)
	, m_zoom(-1)
	, m_center(-8.818392, 17.49625)
	, m_scene(new OsmScene(this))
	, m_mouseDown(false)
{
	setScene(m_scene);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	setMinimumSize(400, 400);
	setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	connect(m_scene, &OsmScene::hostActive, this, &OsmView::hostActive);
	connect(m_scene, &OsmScene::selectionChanged, this, &OsmView::selectionChanged);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

OsmView::~OsmView()
{
	emit dying();
}

QList<QGraphicsItem *> OsmView::items(Qt::SortOrder order) const
{
	return m_scene->items(order);
}

void OsmView::unselectAll()
{
	foreach (QGraphicsItem *gi, m_scene->items())
	{
		gi->setSelected(false);
	}
}

void OsmView::addItem(QGraphicsItem *item)
{
	m_scene->addItem(item);
}

void OsmView::fill(QPointF center, int z)
{
	m_center = center;
	centerOn(m_center);
	QTransform trans = m_scene->trans(m_center, m_zoom);
	setTransform(trans);
	centerOn(m_center);
	QPointF	tl = mapToScene(rect().topLeft());
	QPointF	br = mapToScene(rect().bottomRight());
	m_geoRect = QRectF(tl, br);
//	qDebug() << Q_FUNC_INFO << rect() << m_geoRect << mapToScene(rect()).boundingRect();
	if (m_zoom != z)
	{
		m_zoom = z;
		emit changedZoom(z);
		emit changedTransform(trans);
	}
	m_scene->fill(m_geoRect, m_zoom);
}

void OsmView::incZoom()
{
	setZoom(m_zoom + 1);
}

void OsmView::decZoom()
{
	setZoom(m_zoom - 1);
}

void OsmView::setZoom(int z)
{
//	Q_UNUSED(z)
	fill(m_center, qBound(0, 18, z));
	emit changedTransform(transform());
}

void OsmView::setCenter(QPointF p)
{
	fill(p, m_zoom);
	emit centerChanged(p);
}

void OsmView::resizeEvent(QResizeEvent * event)
{
	QGraphicsView::resizeEvent(event);
	fill(m_center, m_zoom);
}

void OsmView::mouseDoubleClickEvent(QMouseEvent * event)
{
	setCenter(mapToScene(event->pos()));
}

void OsmView::wheelEvent(QWheelEvent * event)
{
	if (event->angleDelta().y() > 0)
		incZoom();
	else
		decZoom();
}

void OsmView::mousePressEvent(QMouseEvent * e)
{
	QGraphicsView::mousePressEvent(e);
	if (! e->isAccepted() && ! m_mouseDown)
	{	m_lastMouse = e->pos();
		m_mouseDown = true;
	}
	emit mousePos(mapToScene(e->pos()));
}

void OsmView::mouseMoveEvent(QMouseEvent * e)
{
	QGraphicsView::mouseMoveEvent(e);
	if (m_mouseDown)
	{
		QPoint p = mapFromScene(m_center);
		QPointF pp = mapToScene(p + (m_lastMouse - e->pos()));
		setCenter(pp);
		emit mousePos(pp);

		m_lastMouse = e->pos();
	}
}

void OsmView::mouseReleaseEvent(QMouseEvent * e)
{
	m_mouseDown = false;
	QGraphicsView::mouseReleaseEvent(e);
	emit mousePos(mapToScene(e->pos()));
}

void OsmView::ensureVisible(const QGraphicsItem * item)
{
	QGraphicsView::ensureVisible(item, 100, 100);
	fillView();
}

void OsmView::ensureVisible(const QPointF p)
{
//	qDebug() << Q_FUNC_INFO << p;
	QGraphicsView::ensureVisible(QRectF(p, p), 100, 100);
	fillView();
}

void OsmView::fillView()
{
	QPointF p = mapToScene(rect().center());
	setCenter(p);
}



