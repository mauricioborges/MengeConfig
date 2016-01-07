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
	QAction * action = new QAction(QIcon(":/images/toggleAxis.png"), tr("Toggle &Axis"), this);
	action->setCheckable(true);
	action->setChecked(true);
	action->setToolTip(tr("Toggle the display of the axis in the scene viewer"));
	_toolBar->addAction(action);
	connect(action, &QAction::triggered, _glView, &GLWidget::setDrawAxis);
	

	setLayout(mainLayout);
}