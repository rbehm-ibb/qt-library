/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "ibtoolbar.h"
#include "ibtoolbarplugin.h"

#include <QtPlugin>

//! [0]
IbToolBarPlugin::IbToolBarPlugin(QObject *parent)
	: QObject(parent)
	, initialized(false)
{
}
//! [0]

//! [1]
void IbToolBarPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (initialized)
		return;

	initialized = true;
}
//! [1]

//! [2]
bool IbToolBarPlugin::isInitialized() const
{
	return initialized;
}
//! [2]

//! [3]
QWidget *IbToolBarPlugin::createWidget(QWidget *parent)
{
	return new IBToolBar(parent);
}
//! [3]

//! [4]
QString IbToolBarPlugin::name() const
{
	return "IBToolBar";
}
//! [4]

//! [5]
QString IbToolBarPlugin::group() const
{
	return "Display Widgets [Examples]";
}
//! [5]

//! [6]
QIcon IbToolBarPlugin::icon() const
{
	return QIcon(":/logo.png");
}
//! [6]

//! [7]
QString IbToolBarPlugin::toolTip() const
{
	return "";
}
//! [7]

//! [8]
QString IbToolBarPlugin::whatsThis() const
{
	return "";
}
//! [8]

//! [9]
bool IbToolBarPlugin::isContainer() const
{
	return false;
}
//! [9]

//! [10]
QString IbToolBarPlugin::domXml() const
{
	return "<ui language=\"c++\">\n"
	       " <widget class=\"IBToolBar\" name=\"ibToolBar\">\n"
	       "  <property name=\"toolTip\" >\n"
	       "   <string>OSM map</string>\n"
	       "  </property>\n"
	       "  <property name=\"whatsThis\" >\n"
	       "   <string>A toolbar with auto Quit and About actions.</string>\n"
	       "  </property>\n"
	       " </widget>\n"
	       "</ui>\n";
}
//! [10]

//! [12]
QString IbToolBarPlugin::includeFile() const
{
	return "ibtoolbar.h";
}
//! [12]
