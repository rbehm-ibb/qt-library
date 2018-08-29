#ifndef QHEXVIEW_H_
#define QHEXVIEW_H_


class QHexView: public QAbstractScrollArea
{
	Q_OBJECT
public:
	QHexView(QWidget *parent = 0);

	QByteArray data() const;

signals:
	void cursorIndex(int idx);

public slots:
	void setData(const QByteArray &arr);
	void clear();
	void showFromOffset(int offset);

protected:
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
private:
	QByteArray	m_data;
	int	m_posAddr;
	int	m_posHex;
	int	m_posAscii;
	int	m_charWidth;
	int	m_charHeight;


	int	m_selectBegin;
	int	m_selectEnd;
	int	m_selectInit;
	int	m_cursorPos;


	QSize fullSize() const;
	void resetSelection();
	void resetSelection(int pos);
	void setSelection(int pos);
	void ensureVisible();
	void setCursorPos(int pos);
	int cursorPos(const QPoint &position);
};

#endif
