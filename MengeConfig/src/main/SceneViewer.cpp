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
	mainLayout->addWidget(_toolBar);

	_glView = new GLWidget();
	mainLayout->addWidget(_glView);

	// Populate tool bar
	QAction * togAxisAct = new QAction(QIcon(":/images/toggleAxis.png"), tr("Toggle &Axis"), this);
	togAxisAct->setCheckable(true);
	togAxisAct->setChecked(true);
	togAxisAct->setToolTip(tr("Toggle the display of the axis in the scene viewer"));
	_toolBar->addAction(togAxisAct);
	connect(togAxisAct, &QAction::triggered, _glView, &GLWidget::setDrawAxis);

	QAction * togPerspAct = new QAction(QIcon(":/images/togglePersp.png"), tr("Toggle &Perspective"), this);
	togPerspAct->setCheckable(true);
	togPerspAct->setChecked(true);
	togPerspAct->setToolTip(tr("Toggle the current camera's projection between perspective and orthographic"));
	_toolBar->addAction(togPerspAct);
	connect(togPerspAct, &QAction::triggered, _glView, &GLWidget::toggleProjection);
	

	setLayout(mainLayout);
}