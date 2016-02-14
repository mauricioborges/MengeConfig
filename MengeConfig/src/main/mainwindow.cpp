#include "mainwindow.hpp"

#include "AppLogger.hpp"
#include "ContextManager.hpp"
#include "FSMViewer.hpp"
#include "SceneHierarchy.hpp"
#include "SceneViewer.hpp"
#include "SceneHierarchy.hpp"
#include "ToolProperties.hpp"

#include <QtWidgets\qboxlayout.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSplitter>

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of MainWindow
/////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow()
{
	// The major gui components
	QWidget * widget = new QWidget();
	QSplitter * vSplitter = new QSplitter();
	vSplitter->setOrientation(Qt::Vertical);
	
	QSplitter * splitter = new QSplitter();
	splitter->setContentsMargins(0, 0, 0, 0);

	_sceneViewer = new SceneViewer();
	splitter->addWidget(_sceneViewer);

	_fsmViewer = new FSMViewer();
	splitter->addWidget(_fsmViewer);

	vSplitter->addWidget(splitter);
	setCentralWidget(vSplitter);
	
	buildMenu();

	// Docked elements
	_hierarchyDock = new QDockWidget(tr("Scene Hierarchy"), this);
	_hierarchy = new SceneHierarchy();
	_hierarchyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_hierarchyDock->setWidget(_hierarchy);
	addDockWidget(Qt::RightDockWidgetArea, _hierarchyDock);
	connect(_hierarchyDock, &QDockWidget::visibilityChanged, this, &MainWindow::toggleHierarchy);

	_toolPropDock = new QDockWidget(tr("Tool Properties"), this);
	_toolProperties = new ToolPropertyWidget();
	_toolPropDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_toolPropDock->setWidget(_toolProperties);
	addDockWidget(Qt::LeftDockWidgetArea, _toolPropDock);
	connect(_toolPropDock, &QDockWidget::visibilityChanged, this, &MainWindow::toggleToolProperties);

	ContextManager * mgr = ContextManager::instance();
	connect(mgr, &ContextManager::activated, _toolProperties, &ToolPropertyWidget::activated);
	connect(mgr, &ContextManager::deactivated, _toolProperties, &ToolPropertyWidget::deactivated);
	
	// Set up the logger
	_logger = new AppLogger(this);
	_logger->setVisible(false);
	vSplitter->addWidget(_logger);

}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::buildMenu() {
	QMenuBar *menuBar = new QMenuBar;

	// Obstacles menu
	QMenu * menuObst = menuBar->addMenu(tr("&Obstacles"));

	_drawObstacleAct = new QAction(menuObst);
	_drawObstacleAct->setText(tr("&Draw Obstacle"));
	menuObst->addAction(_drawObstacleAct);
	connect(_drawObstacleAct, &QAction::triggered, _sceneViewer, &SceneViewer::drawObstacle);


	// View menu
	QMenu *menuView = menuBar->addMenu(tr("&View"));

	_toggleSceneVis = new QAction(menuView);
	_toggleSceneVis->setText(tr("Scene Viewer"));
	_toggleSceneVis->setCheckable(true);
	_toggleSceneVis->setChecked(true);
	menuView->addAction(_toggleSceneVis);
	connect(_toggleSceneVis, &QAction::triggered, this, &MainWindow::toggleSceneViewer);

	_toggleSceneHierarchy = new QAction(menuView);
	_toggleSceneHierarchy->setText(tr("Scene Hierarchy"));
	_toggleSceneHierarchy->setCheckable(true);
	_toggleSceneHierarchy->setChecked(true);
	menuView->addAction(_toggleSceneHierarchy);
	connect(_toggleSceneHierarchy, &QAction::triggered, this, &MainWindow::toggleHierarchy);

	_toggleFsmVis = new QAction(menuView);
	_toggleFsmVis->setText(tr("FSM Viewer"));
	_toggleFsmVis->setCheckable(true);
	_toggleFsmVis->setChecked(true);
	menuView->addAction(_toggleFsmVis);
	connect(_toggleFsmVis, &QAction::triggered, this, &MainWindow::toggleFSMViewer);

	_toggleLogVis = new QAction(menuView);
	_toggleLogVis->setText(tr("Activity Log"));
	_toggleLogVis->setCheckable(true);
	_toggleLogVis->setChecked(false);
	menuView->addAction(_toggleLogVis);
	connect(_toggleLogVis, &QAction::triggered, this, &MainWindow::toggleLog);

	_toggleToolProperties = new QAction(menuView);
	_toggleToolProperties->setText(tr("Tool Properties"));
	_toggleToolProperties->setCheckable(true);
	_toggleToolProperties->setChecked(false);
	menuView->addAction(_toggleToolProperties);
	connect(_toggleToolProperties, &QAction::triggered, this, &MainWindow::toggleToolProperties);

	setMenuBar(menuBar);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::toggleHierarchy(bool state) {
	// NOTE: This does not cause a feedback loop with hierarchyVisibilityChanged because
	//	only if the visibility state *changes* does work get done.
	_hierarchyDock->setVisible(state);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::hierarchyVisibilityChanged(bool state) {
	// NOTE: This does not cause a feedback loop with toggleHierarchy because
	//	only if the visibility state *changes* does work get done.
	_toggleSceneHierarchy->setChecked(state);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::toggleToolProperties(bool state) {
	// NOTE: This does not cause a feedback loop with hierarchyVisibilityChanged because
	//	only if the visibility state *changes* does work get done.
	_toolPropDock->setVisible(state);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::toolPropertiesVisibilityChanged(bool state) {
	// NOTE: This does not cause a feedback loop with toggleHierarchy because
	//	only if the visibility state *changes* does work get done.
	_toggleToolProperties->setChecked(state);
}


void MainWindow::toggleSceneViewer(bool state) {
	_sceneViewer->setVisible(state);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::toggleFSMViewer(bool state) {
	_fsmViewer->setVisible(state);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::toggleLog(bool state) {
	_logger->setVisible(state);
}