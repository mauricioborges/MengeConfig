#include "SceneViewer.hpp"

#include "glwidget.hpp"

#include <QtWidgets/qaction.h>
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qToolbar.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SceneViewer
/////////////////////////////////////////////////////////////////////////////////////////////

SceneViewer::SceneViewer(QWidget * parent) : QWidget(parent) {
	QVBoxLayout * mainLayout = new QVBoxLayout();

	_toolBar = new QToolBar();
	mainLayout->addWidget(_toolBar);
	mainLayout->setMargin(0);

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

	QComboBox * dirComboBox = new QComboBox();
	dirComboBox->setEditable(false);
	QStringList directions;
	directions << "x" << "-x" << "y" << "-y" << "z" << "-z";
	dirComboBox->addItems(directions);
	dirComboBox->setCurrentIndex(5);
	_toolBar->addSeparator();
	_toolBar->addWidget(dirComboBox);
	_toolBar->addSeparator();
	// TODO: Figure out why the function pointer version didn't work and I had to use the old slot/signal crap.
	//connect(dirComboBox, &QComboBox::currentIndexChanged, _glView, &GLWidget::setViewDirection);
	connect(dirComboBox, SIGNAL(activated(int)), _glView, SLOT(setViewDirection(int)));

	QAction * togGridAct = new QAction(QIcon(":/images/toggleGrid.png"), tr("Toggle &Grid"), this);
	togGridAct->setCheckable(true);
	togGridAct->setChecked(true);
	togGridAct->setToolTip(tr("Toggle the reference grid; an inactive grid cannot be used for snapping."));
	_toolBar->addAction(togGridAct);
	connect(togGridAct, &QAction::triggered, this, &SceneViewer::toggleGrid);

	QAction * gridPropAct = new QAction(QIcon(":/images/gridProperties.png"), tr("Grid &Properties"), this);
	gridPropAct->setToolTip(tr("Edit the reference grid's properties."));
	_toolBar->addAction(gridPropAct);
	connect(gridPropAct, &QAction::triggered, _glView, &GLWidget::editGridProperties);

	setLayout(mainLayout);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::toggleGrid(bool state) {
	_glView->toggleReferenceGrid(state);
	// TODO: Disable snapping buttons
}