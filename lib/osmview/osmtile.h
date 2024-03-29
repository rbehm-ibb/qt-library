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
	static QString path() { return m_path; }
	static bool fetchTiles() { return m_fetchTiles; }
	static void setFetchTiles(bool fetchTiles) { m_fetchTiles = fetchTiles; }
	void get(QPointF, uint z);
	void get(uint ix, uint iy, uint z);
	void setParm(uint ix, uint iy, uint z);
	static QString getTileserver() { return m_tileserver; }
	static void setTileserver(const QString &tileserver) { m_tileserver = tileserver; }
	static QNetworkAccessManager *networkManager();

	static QString mapboxTileset() { return m_mapboxTileset; }
	static void setMapboxTileset(const QString &mapboxTileset) { m_mapboxTileset = mapboxTileset; }

	static QByteArray getUserAgent() { return m_userAgent; }
	static void setUserAgent(const QString &userAgent) { m_userAgent = userAgent.toLatin1(); }

	bool setPix(const QString fn);
	QGraphicsPixmapItem *toGraphic(const QPixmap pm = QPixmap());

	QGraphicsPixmapItem *pmi() const { return m_pmi; }
	quint32 ix() const { return m_ix; }
	quint32 iy() const { return m_iy; }

signals:
	void loaded(OsmTile *);

private slots:
	void dataLoaded();
	void error(QNetworkReply::NetworkError code);
private:
	static QNetworkAccessManager * m_networkManager;
	static QNetworkDiskCache *m_cache;
	QPointer<QNetworkReply> m_qnr;
	static QString m_path;
	static QString m_tileserver;
	static QString m_mapboxTileset;
	static bool m_fetchTiles;
	static QByteArray m_userAgent;
	QRectF m_geoRect;
	quint16 m_zoom;
	quint32 m_ix;
	quint32 m_iy;
	QPixmap m_pix;
	static QRectF calcRect(uint ix, uint iy, uint z);
	QGraphicsPixmapItem *m_pmi;
};


#endif // OSMTILE_H
