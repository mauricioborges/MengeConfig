#include "SceneViewer.hpp"

#include "glwidget.hpp"

#include <QtWidgets/qToolbar.h>
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qaction.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SceneViewer
/////////////////////////////////////////////////////////////////////////////////////////////

SceneViewer::SceneViewer(QWidget * parent) : QWidget(parent) {
	QVBoxLayout * mainLayout = new QVBoxLayout();
	_toolBar = new QToolBar();
	QAction * action = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
	_toolBar->addAction(action);
	mainLayout->addWidget(_toolBar);
	_glView = new GLWidget();
	mainLayout->addWidget(_glView);

	setLayout(mainLayout);
}