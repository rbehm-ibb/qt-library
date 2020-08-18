// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef OSMSCENE_H
#define OSMSCENE_H

#include "osmtile.h"

class OsmScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit OsmScene(QObject *parent = nullptr);
	~OsmScene();
	void fill(QRectF, int z);
	QTransform trans(QPointF center, int z);
	QList<QGraphicsItem *> items(Qt::SortOrder order = Qt::DescendingOrder) const;
	void addItem(QGraphicsItem *item);
signals:
	void hostActive(bool);
//	void dying();
public slots:
private slots:
	void tileLoaded(OsmTile *);
protected:
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
private:
	static QString tag(int x, int y, int z);
	typedef QMap<QString, QGraphicsPixmapItem*> TileMap;
	TileMap m_tiles;
};

#endif // OSMSCENE_H
