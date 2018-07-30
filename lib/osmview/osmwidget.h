// ******************************************************
// * copyright (C) 2018 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * File: osmwidget.h    OsmWidget
// * created 12/9/2014 by behm
// ******************************************************

#ifndef OSMWIDGET_H
#define OSMWIDGET_H

class OsmView;
class MapScaleWidget;

#if 0
// init example
ui->mapWidget->setZoom(Config::intValue("/map/zoom"));
ui->mapWidget->setHome(QPointF(Config::value("/map/home").toPoint()) * 1e-5);
ui->statusBar->addPermanentWidget(ui->mapWidget->posLabel());
ui->statusBar->addPermanentWidget(ui->mapWidget->mapHostLabel());
ui->statusBar->addPermanentWidget(new QLabel(ui->mapWidget->copyright()));

void MainWindow::on_actionSetup_triggered()
{
	Config::setValue("/map/home", (ui->mapWidget->center()*1e5).toPoint());
	Config::setValue("/map/zoom", ui->mapWidget->zoom());
}
#endif

class OsmWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int zoom READ zoom WRITE setZoom)
	Q_PROPERTY(QString tileserver READ tileserver WRITE setTileserver)
	Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged)
	Q_PROPERTY(bool fetchTiles READ fetchTiles WRITE setFetchTiles)
public:
	explicit OsmWidget(QWidget *parent = 0);
	OsmView *view() const { return m_osmView; }
	QList<QGraphicsItem *> items() const;
	void addItem(QGraphicsItem *item);
	static void setPath(QString path);
	static void setFetchTiles(bool fetchTiles);
	static bool fetchTiles();
	QToolBar *toolBar() const { return m_toolbar; }
	QLabel *mapHostLabel() const { return m_mapHost; }
	QString copyright() const;
	QLabel *posLabel() const { return m_pos; }
	QAction *actZoomP() const { return m_actZoomP; }
	QAction *actZoomM() const { return m_actZoomM; }
	QPointF center() const;
	void ensureVisible(QPointF pos);
	void unselectAll();
	static QNetworkAccessManager *networkManager();
	void setTileserver(const QString tileServer);
	QString tileserver() const;
	int zoom() const;

signals:
	void centerChanged(QPointF center);
	void changedTransform(const QTransform t);
public slots:
	void setHome(QPointF p);
	void goHome();
	void setCenter(QPointF p);
	void setZoom(int z);
private slots:
	void changedZoom(int z);
	void mapHostActive(bool on);
	void mousePosOsmSlot(QPointF p);

private:
	QToolBar *m_toolbar;
	OsmView *m_osmView;
	QAction *m_actZoomP;
	QAction *m_actZoomM;
	QSlider *m_zoomSlider;
	QAction *m_actHome;
	QLabel *m_mapHost;
	QLabel *m_pos;
	QPointF m_homePos;
	MapScaleWidget *m_scale;
};

#endif // OSMWIDGET_H
