// ******************************************************
// * copyright (C) 2016 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 11/22/2016 by behm
// ******************************************************

#include "fileselector.h"

FileSelector::FileSelector(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(1);
	m_name = new QLineEdit(this);
	m_selButton = new QPushButton(this);
	m_selButton->setIcon(QIcon(":/fileselector/fileopen.svgz"));
	m_viewButton = new QPushButton(this);
	m_viewButton->setIcon(QIcon(":/fileselector/eye.png"));
	layout->addWidget(m_name);
	layout->addWidget(m_selButton);
	layout->addWidget(m_viewButton);
	connect(m_selButton, &QPushButton::clicked, this, &FileSelector::selectFile);
	connect(m_viewButton, &QPushButton::clicked, this, &FileSelector::viewFile);
}

void FileSelector::setFn(const QString fn)
{
	m_name->setText(fn);
}

void FileSelector::setDir(const QString dir)
{
	m_homeDir = dir;
}

void FileSelector::selectFile()
{
	qDebug() << Q_FUNC_INFO;
	QString fn = QFileDialog::getOpenFileName(this, tr("Select Document"), m_name->text());
	if (! fn.isEmpty())
	{
		setFn(fn);
	}
}

void FileSelector::viewFile()
{
	qDebug() << Q_FUNC_INFO;
	QDesktopServices::openUrl(fileName());
}

