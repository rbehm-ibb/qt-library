// ******************************************************
// * copyright (C) 2019 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/15/2019 by behm
// ******************************************************

#include "serialportselector.h"

SerialPortSelector::SerialPortSelector(QWidget *parent)
	: QComboBox(parent)
{
	foreach (auto & port, QSerialPortInfo::availablePorts())
	{
		addItem(port.portName() + " " + port.description(), port.portName());
	}
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

