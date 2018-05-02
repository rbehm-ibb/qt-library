// ******************************************************
// * copyright (C) 2014 by Becker Electronics Taiwan Ltd.
// * All Rights reserved
// * File: mapscalewidget.h    MapScaleWidget
// * created 9/29/2014 by behm
// ******************************************************

#ifndef MAPSCALEWIDGET_H
#define MAPSCALEWIDGET_H

class MapScaleWidget : public QLabel
{
	Q_OBJECT
public:
	/// Create a MapScaleWidget
	/// \param parent for QObject
	explicit MapScaleWidget(QWidget *parent = 0);

signals:

public slots:
	void changedScale(QTransform trans);
//	void scaleChanged(const qreal scale);	///< We have a new scale, [deg/pixel]
private:
	qreal m_scale;		///< the scale value in Y dir
	void createImage();	///< create the image for the scale
	QFont font;		///< the font we use.
};

#endif // MAPSCALEWIDGET_H
