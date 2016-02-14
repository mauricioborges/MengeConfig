#include "SceneViewer.hpp"

#include "ContextManager.hpp"
#include "glwidget.hpp"
#include "ObstacleContext.hpp"

#include <QtWidgets/qaction.h>
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/QLabel.h>
#include <QtWidgets/qToolbar.h>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SceneViewer
/////////////////////////////////////////////////////////////////////////////////////////////

SceneViewer::SceneViewer(QWidget * parent) : QWidget(parent) {
	QVBoxLayout * mainLayout = new QVBoxLayout();

	_toolBar = new QToolBar();
	mainLayout->addWidget(_toolBar, 0);
	mainLayout->setMargin(0);

	_glView = new GLWidget();
	mainLayout->addWidget(_glView, 1);

	_statusLabel = new QLabel("Scene Viewer:");
	QHBoxLayout * statusLayout = new QHBoxLayout();
	statusLayout->setContentsMargins(5, 1, 5, 1);
	statusLayout->addWidget(_statusLabel, 1);

	_posLabel = new QLabel("no point");
	statusLayout->addWidget(_posLabel, 0, Qt::AlignRight);

	mainLayout->addLayout(statusLayout, 0);

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

	_dirComboBox = new QComboBox();
	_dirComboBox->setEditable(false);
	QStringList directions;
	directions << "user-defined" << "x" << "-x" << "y" << "-y" << "z" << "-z";
	_dirComboBox->addItems(directions);
	_dirComboBox->setCurrentIndex(5);
	_toolBar->addSeparator();
	_toolBar->addWidget(_dirComboBox);
	_toolBar->addSeparator();
	// TODO: Figure out why the function pointer version didn't work and I had to use the old slot/signal crap.
	//connect(dirComboBox, &QComboBox::currentIndexChanged, _glView, &GLWidget::setViewDirection);
	connect(_dirComboBox, SIGNAL(activated(int)), _glView, SLOT(setViewDirection(int)));

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

	_gridHSnap = new QAction(QIcon(":/images/gridSnapHorizontal.png"), tr("&Horizontal Snap"), this);
	_gridHSnap->setCheckable(true);
	_gridHSnap->setChecked(false);
	_gridHSnap->setToolTip(tr("Causes mouse selection points to snap to horizontal grid lines."));
	_toolBar->addAction(_gridHSnap);
	connect(_gridHSnap, &QAction::triggered, _glView, &GLWidget::toggleHorizontalSnap);

	_gridVSnap = new QAction(QIcon(":/images/gridSnapVertical.png"), tr("&Vertical Snap"), this);
	_gridVSnap->setCheckable(true);
	_gridVSnap->setChecked(false);
	_gridVSnap->setToolTip(tr("Causes mouse selection points to snap to vertical grid lines."));
	_toolBar->addAction(_gridVSnap);
	connect(_gridVSnap, &QAction::triggered, _glView, &GLWidget::toggleVerticalSnap);

	connect(_glView, &GLWidget::userRotated, this, &SceneViewer::userRotated);
	connect(_glView, &GLWidget::currWorldPos, this, &SceneViewer::setCurrentWorldPos);

	setLayout(mainLayout);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::drawObstacle() {
	// TODO: Determine where the context comes from.
	QtContext * ctx = new ObstacleContext();
	ContextManager::instance()->activate(ctx);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::toggleGrid(bool state) {
	_glView->toggleReferenceGrid(state);
	_gridHSnap->setEnabled(state);
	_gridVSnap->setEnabled(state);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::updateStatus() {
	// TODO: Write out various state to this line -- e.g., what is selected, etc.
	// TODO: This functionality should ultimately be shared by each of the dialogs.
	QString str("Scene Viewer - ");

	_statusLabel->setText(str);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::userRotated() {
	_dirComboBox->setCurrentIndex(0);
	_posLabel->setText("no point");
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::setCurrentWorldPos(float x, float y) {
	_posLabel->setText(QString("(%1, %2)").arg(x, 0, 'f', 2).arg(y, 0, 'f', 2));
}