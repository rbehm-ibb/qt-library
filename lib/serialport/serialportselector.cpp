// ******************************************************
// * copyright (C) 2019 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/15/2019 by behm
// ******************************************************

#include "serialportselector.h"

SerialPortSelector::SerialPortSelector(QWidget *parent)
	: QComboBox(parent)
{
	setSizeAdjustPolicy(QComboBox::AdjustToContents);
	QAction *refr = new QAction(QIcon("://stdicons/arrow_refresh.png"), "Refresh", this);
	setContextMenuPolicy(Qt::ActionsContextMenu);
	addAction(refr);
	connect(refr, &QAction::triggered, this, &SerialPortSelector::refresh);
	refresh();
}

SerialPortSelector::~SerialPortSelector()
{
}

void SerialPortSelector::init(const QString port)
{
	const int idx = findData(port);
	if (idx >= 0)
	{
		setCurrentIndex(idx);
	}
	emit portChanged(portname());
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(on_currentIndexChanged(int)));
}

QString SerialPortSelector::portname() const
{
	return currentData().toString();
}

void SerialPortSelector::on_currentIndexChanged(int index)
{
	emit portChanged(itemData(index).toString());
}

void SerialPortSelector::refresh()
{
	clear();
	foreach (auto & port, QSerialPortInfo::availablePorts())
	{
		addItem(port.portName() + " " + port.manufacturer() + " " + port.description(), port.portName());
	}

}

