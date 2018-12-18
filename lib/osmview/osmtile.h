// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#ifndef OSMTILE_H
#define OSMTILE_H

class OsmTile : public QObject
{
	Q_OBJECT
public:
	explicit OsmTile(QObject *parent = nullptr);
	~OsmTile();
	uint zoom() const { return m_zoom; }
	uint idxX() const { return m_ix; }
	uint idxY() const { return m_iy; }
	QRectF geoRect() const { return m_geoRect; }
	QPixmap pix() const { return m_pix; }
	static QPoint idxFromCoord(QPointF, uint);
	static QRectF calcRect(QPointF, uint z);
	static void setPath(QString path);
	static bool fetchTiles() { return m_fetchTiles; }
	static void setFetchTiles(bool fetchTiles) { m_fetchTiles = fetchTiles; }
	void get(QPointF, uint z);
	void get(uint ix, uint iy, uint z);
	static QString getTileserver() { return m_tileserver; }
	static void setTileserver(const QString &tileserver) { m_tileserver = tileserver; }
	static QNetworkAccessManager *networkManager();

signals:
	void loaded(OsmTile *);

private slots:
	void dataLoaded();
private:
	static QNetworkAccessManager * m_networkManager;
	static QNetworkDiskCache *m_cache;
	QPointer<QNetworkReply> m_qnr;
	static QString m_path;
	static QString m_tileserver;
	static bool m_fetchTiles;
	QRectF m_geoRect;
	quint16 m_zoom;
	quint32 m_ix;
	quint32 m_iy;
	QPixmap m_pix;
	static QRectF calcRect(uint ix, uint iy, uint z);
};


#endif // OSMTILE_H
