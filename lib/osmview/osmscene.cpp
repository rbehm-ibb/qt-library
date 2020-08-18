// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#include "osmscene.h"

OsmScene::OsmScene(QObject *parent)
	: QGraphicsScene(QRectF(-180, 90, 360, -180), parent)
{
}

OsmScene::~OsmScene()
{
//	emit dying();
}

void OsmScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	QGraphicsItem * item = mouseGrabberItem();
	if (item && (item->flags() & QGraphicsItem::ItemIsMovable))
	{
		QGraphicsScene::mouseMoveEvent(mouseEvent);
		mouseEvent->accept();
	}
	else
	{
		mouseEvent->ignore();
	}
//	qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
}

void OsmScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
//	qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
	QGraphicsScene::mousePressEvent(mouseEvent);
}

QTransform OsmScene::trans(QPointF p, int z)
{
	QRectF rr = OsmTile::calcRect(p, z);
	QTransform trans;
	QPointF tl  = rr.topLeft();
	QPointF br = rr.bottomRight();
	QPointF dd = br - tl;
	trans.scale(256./dd.x(), 256./dd.y());
	return trans;
}

QList<QGraphicsItem *> OsmScene::items(Qt::SortOrder order) const
{
	QList<QGraphicsItem *> list;
	foreach (QGraphicsItem *item, QGraphicsScene::items(order))
	{
		QGraphicsPixmapItem *pi = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);
		if (! m_tiles.values().contains(pi))
		{
			list += item;
		}
	}
//	qDebug() << Q_FUNC_INFO << list;
	return list;
}

void OsmScene::addItem(QGraphicsItem *item)
{
//	QTransform t;
//	t.scale(1, -1);
//	item->setTransform(t);
	QGraphicsScene::addItem(item);
}

void OsmScene::fill(QRectF r, int z)
{
//	vrect = r;
	QPoint tl = OsmTile::idxFromCoord(r.topLeft(), z);
	QPoint br = OsmTile::idxFromCoord(r.bottomRight(), z);
//	qDebug() << Q_FUNC_INFO << r << z << tl << br;
	int max = int(pow(2.0, z)) - 1;
	int ixl = qBound(0, max, tl.x());
	int ixr = qBound(0, max, br.x());
	int iyl = qBound(0, max, tl.y());
	int iyr = qBound(0, max, br.y());
	TileMap used = m_tiles;
	m_tiles.clear();
	for (int ix = ixl; ix <= ixr; ++ix)
	{
		for (int iy = iyl; iy <= iyr; ++iy)
		{
			QString ttag = tag(ix, iy, z);
			TileMap::iterator it = used.find(ttag);
			if (it != used.end())
			{
				m_tiles.insert(ttag, it.value());
				used.remove(ttag);
			}
			else
			{
				m_tiles.insert(ttag, 0);
				OsmTile * osm = new OsmTile(this);
				connect(osm, &OsmTile::loaded, this, &OsmScene::tileLoaded);
				emit hostActive(true);
				osm->get(ix, iy, z);
			}
		}
	}
	// delete unused tiles
	qDeleteAll(used);
}

QString OsmScene::tag(int x, int y, int z)
{
	return QString("%1_%2_%3").arg(z).arg(x).arg(y);
}

void OsmScene::tileLoaded(OsmTile * osm)
{
	QString ttag = tag(osm->idxX(), osm->idxY(), osm->zoom());
	TileMap::iterator it = m_tiles.find(ttag);
	if (it != m_tiles.end())
	{
		if (it.value())
			delete it.value();
		QGraphicsPixmapItem * gi = addPixmap(osm->pix());
		it.value() = gi;
		qreal sx = osm->geoRect().width() / osm->pix().width();
		qreal sy = osm->geoRect().height() / osm->pix().height();
		QTransform tm;
		tm.scale(sx, sy);
//		qDebug() << tm;
		gi->setTransform(tm);
		gi->setPos(osm->geoRect().topLeft());
		gi->setZValue(-1);
//		gi->setOpacity(0.3);
	}
	else
	{
//		qWarning() << Q_FUNC_INFO << "unneeded Tile" << ttag;
	}
	osm->deleteLater();
	bool anyRequest = false;
	foreach (QGraphicsPixmapItem * tile, m_tiles)
	{
		if (! tile)
		{
			anyRequest = true;
			break;
		}
	}
//	if (!anyRequest)
//		qDebug() << Q_FUNC_INFO << m_tiles.count() << anyRequest;
	emit hostActive(anyRequest);
}
