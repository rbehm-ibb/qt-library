// ******************************************************
// * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// ******************************************************

#include "osmtile.h"

// #define MAPBOX

#ifdef MAPBOX
#include "setting.h"
#endif

QNetworkAccessManager * OsmTile::m_networkManager = nullptr;
QNetworkDiskCache *OsmTile::m_cache = nullptr;
QString OsmTile::m_path;
QString OsmTile::m_tileserver("tile.openstreetmap.org");
bool OsmTile::m_fetchTiles = true;

static inline int long2tilex(double lon, uint z)
{
	lon = qBound(-180., 180., lon);
	return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, (int)z)));
}
static inline int lat2tiley(double lat, uint z)
{
	lat = qBound(-90., 90., lat);
	return (int)(floor((1.0 - log(tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, (int)z)));
}
static inline double tilex2lon(uint x, uint z)
{
	return x / pow(2.0, (int)z) * 360.0 - 180;
}
static inline double tiley2lat(uint y, uint z)
{
	double n = M_PI - 2.0 * M_PI * y / pow(2.0, (int)z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

OsmTile::OsmTile(QObject *parent)
	: QObject(parent)
	, m_qnr(0)
{
}

OsmTile::~OsmTile()
{
	if (m_qnr)
	{
		m_qnr->close();
		m_qnr->deleteLater();
	}
}

void OsmTile::setPath(QString path)
{
//	qDebug() << Q_FUNC_INFO << path << cache;
	m_path = path;
	if (m_path.isEmpty())
	{
		if (! m_cache)
		{
			m_cache = new QNetworkDiskCache();
			m_cache->setCacheDirectory(QDir::homePath() + "/.cache/osm/");
//			qDebug() << m_cache->cacheDirectory() << m_networkManager;
			networkManager()->setCache(m_cache);
		}
	}
	else
	{
		if (m_cache)
		{
			networkManager()->setCache(nullptr);
//			m_cache->deleteLater();	// FIXME deleting cache crashes
			m_cache = nullptr;
		}
	}
}

QPoint OsmTile::idxFromCoord(QPointF p, uint z)
{
	int ix = long2tilex(p.x(), z);
	int iy = lat2tiley(p.y(), z);
	return QPoint(ix, iy);
}

QRectF OsmTile::calcRect(QPointF p, uint z)
{
	return calcRect(long2tilex(p.x(), z), lat2tiley(p.y(), z), z);
}

QRectF OsmTile::calcRect(uint ix, uint iy, uint z)
{
	QRectF r;
	r.setTop(tiley2lat(iy, z));
	r.setBottom(tiley2lat(iy + 1, z));
	r.setLeft(tilex2lon(ix, z));
	r.setRight(tilex2lon(ix + 1, z));
	return r;
}

void OsmTile::get(QPointF tl, uint z)
{
//	qDebug() << Q_FUNC_INFO << idxFromCoord(tl, z);
	get(long2tilex(tl.x(), z), lat2tiley(tl.y(), z), z);
}

void OsmTile::get(uint ix, uint iy, uint z)
{
	m_zoom = z;
	m_ix = ix;
	m_iy = iy;

	m_geoRect = calcRect(m_ix, m_iy, m_zoom);
	m_pix = QPixmap();
	QString tag(QString("/%1/%2_%3").arg(z).arg(ix).arg(iy));
	QString fileName(m_path + tag + ".png");
//	qDebug() << Q_FUNC_INFO << fileName;
	if (! m_path.isEmpty() && m_pix.load(fileName))
	{
//		qDebug() << Q_FUNC_INFO << fileName;
		emit loaded(this);
//		pi = new QGraphicsPixmapItem(m_pix);
	}
	else if (m_fetchTiles)
	{
#ifdef MAPBOX
		QUrl url;
		url.setUrl(QString("https://api.mapbox.com/v4/mapbox.streets-satellite/%1/%2/%3.png")
			   .arg(m_zoom)
			   .arg(m_ix)
			   .arg(m_iy)
			  );
		url.setUrl(url.toEncoded());
		url.setUrl(url.toEncoded());
		QUrlQuery qry;
		qry.addQueryItem("access_token", Setting::i()->accessToken);
		url.setQuery(qry);
#else
		static const QString tileName("http://%1/%2/%3/%4.png");
		QString url = tileName.arg("tile.openstreetmap.org").arg(m_zoom).arg(m_ix).arg(m_iy);
//		QNetworkRequest req(url);
#endif
		QNetworkRequest req(url);
		req.setRawHeader("user-agent", (qApp->applicationName() + " " + qApp->applicationVersion()).toLatin1());
//		qDebug() << Q_FUNC_INFO << m_fetchTiles << url;
		m_qnr = networkManager()->get(req);
		connect(m_qnr.data(), &QNetworkReply::finished, this, &OsmTile::dataLoaded);
	}
}

QNetworkAccessManager *OsmTile::networkManager()
{
	if (! m_networkManager)
	{
		m_networkManager = new QNetworkAccessManager();
//		qDebug() << Q_FUNC_INFO << "created QNAM" << networkManager;
		if (m_path.isEmpty())
		{
			setPath(m_path);	// create cache
		}
	}
	return m_networkManager;
}

void OsmTile::dataLoaded()
{
	//	qDebug() << Q_FUNC_INFO << m_qnr;
	if (! m_qnr)
	{
		qDebug() << Q_FUNC_INFO << "Undef";
		return;

	}
	if (m_qnr->error() != QNetworkReply::NoError)
	{
		if (m_qnr->error() != QNetworkReply::OperationCanceledError)
			qWarning() << Q_FUNC_INFO << m_qnr->errorString() << m_qnr->url();
		m_qnr->close();
		m_qnr->deleteLater();
		m_qnr = 0;
		return;
	}
//	qDebug() << Q_FUNC_INFO << cname;
	QUrl url = m_qnr->url();
	QByteArray data = m_qnr->readAll();
	bool ok = m_pix.loadFromData(data);
	if (! ok)
	{
		qWarning() << Q_FUNC_INFO << "bad data" << m_qnr->url();
	}
	else
	{
		emit loaded(this);
		if (! m_path.isEmpty())
		{
			QStringList path = url.path().split("/");	// "" zoom, x, y.png
//			qDebug() << Q_FUNC_INFO << url << data.size() << m_pix.size() << path;

//			qDebug() << Q_FUNC_INFO << m_path << path;
			QDir dir(m_path);
			QString sub = path[1];
			if (! dir.exists(sub))
			{
				qDebug() << Q_FUNC_INFO << "create" << sub;
				dir.mkdir(sub);
			}
			dir.cd(sub);
			m_pix.save(dir.absoluteFilePath(path[2] + "_" + path[3]));
		}
	}
	m_qnr->close();
	m_qnr->deleteLater();
	m_qnr = 0;
}




