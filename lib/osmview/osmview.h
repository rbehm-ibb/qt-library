// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef OSMVIEW_H
#define OSMVIEW_H

class OsmScene;

class OsmView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit OsmView(QWidget *parent = nullptr);
	~OsmView();
	QPointF center() const { return m_center; }
	QRectF geoRect() const { return mapToScene(rect()).boundingRect() /*m_geoRect*/; }
	QList<QGraphicsItem *> items(Qt::SortOrder order = Qt::DescendingOrder) const;
	void addItem(QGraphicsItem *item);
	OsmScene *scene() const { return m_scene; }
	void unselectAll();
	int zoom() const { return m_zoom; }

signals:
	void changedZoom(int);
	void changedTransform(const QTransform t);
	void mousePos(QPointF);
	void hostActive(bool);
	void selectionChanged();
	void centerChanged(QPointF center);
public slots:
	void incZoom();
	void decZoom();
	void setZoom(int);
	void setCenter(QPointF);
	void ensureVisible(const QGraphicsItem * item);
	void ensureVisible(const QPointF);
protected:
	virtual void resizeEvent(QResizeEvent * event);
	virtual void mouseDoubleClickEvent (QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent * event);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void wheelEvent(QWheelEvent * event);
private:
	void fill(QPointF center, int z);
	void fillView();
	int m_zoom;
	QPointF m_center;
	QRectF m_geoRect;
	OsmScene *m_scene;
	bool m_mouseDown;
	QPoint m_lastMouse;
};

#endif // OSMVIEW_H
