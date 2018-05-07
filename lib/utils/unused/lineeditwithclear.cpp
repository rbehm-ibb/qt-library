// ******************************************************
// * copyright (C) 2017 by Reinhardt Behm/rbehm@hushmail.com
// * All Rights reserved
// * created 5/9/2017 by behm
// ******************************************************

#include "lineeditwithclear.h"

LineEditWithClear::LineEditWithClear(QWidget *parent)
	: QWidget(parent)
{
	m_layout = new QHBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_line = new QLineEdit;
	m_layout->addWidget(m_line);
	connect(m_line, &QLineEdit::textChanged, this, &LineEditWithClear::textChanged);
	connect(m_line, &QLineEdit::returnPressed, this, &LineEditWithClear::returnPressed);
	m_clearButton = new QPushButton;
	QPixmap pix(":/stdicons/locationbar_erase.png");
	QTransform trans;
	trans.scale(-1, 1);
	m_clearButton->setIcon(QIcon(pix.transformed(trans)));
	m_clearButton->setToolTip(tr("Clear input"));
	m_clearButton->setFocusPolicy(Qt::NoFocus);
	m_layout->addWidget(m_clearButton);
	connect(m_clearButton, &QPushButton::clicked, m_line, &QLineEdit::clear);
}

void LineEditWithClear::setReadOnly(bool ro)
{
	m_clearButton->setDisabled(ro);
}

void LineEditWithClear::addButton(QPushButton *b)
{
	m_layout->addWidget(b);
}

void LineEditWithClear::addButton(QAction *a)
{
	QPushButton *b = new QPushButton(a->icon(), QString());
	b->setToolTip(a->text());
	m_layout->addWidget(b);
	connect(b, &QPushButton::clicked, a, &QAction::triggered);
}

//void LineEditWithClear::returnPressed()
//{
//	emit textChanged(m_line->text());
//}



