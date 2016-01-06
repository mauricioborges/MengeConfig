
#include "mainwindow.hpp"
#include "FSMViewer.hpp"
#include "SceneHierarchy.hpp"
#include "SceneViewer.hpp"
#include "SceneHierarchy.hpp"

#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSplitter>

#include <iostream>

MainWindow::MainWindow()
{
	// The major gui components
	QSplitter * splitter = new QSplitter();
	_sceneViewer = new SceneViewer();
	splitter->addWidget(_sceneViewer);
	_fsmViewer = new FSMViewer();
	splitter->addWidget(_fsmViewer);
	setCentralWidget(splitter);

	// Set up the main window
    QMenuBar *menuBar = new QMenuBar;
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

	setMenuBar(menuBar);

	// Docked elements
	_hierarchyDock = new QDockWidget(tr("Scene Hierarchy"), this);
	_hierarchy = new SceneHierarchy();
	_hierarchyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	_hierarchyDock->setWidget(_hierarchy);
	addDockWidget(Qt::RightDockWidgetArea, _hierarchyDock);
	connect(_hierarchyDock, &QDockWidget::visibilityChanged, this, &MainWindow::hierarchyVisibilityChanged);
	
}

void MainWindow::toggleHierarchy(bool state) {
	// NOTE: This does not cause a feedback loop with hierarchyVisibilityChanged because
	//	only if the visibility state *changes* does work get done.
	_hierarchyDock->setVisible(state);
}

void MainWindow::hierarchyVisibilityChanged(bool state) {
	// NOTE: This does not cause a feedback loop with toggleHierarchy because
	//	only if the visibility state *changes* does work get done.
	_toggleSceneHierarchy->setChecked(state);
}

void MainWindow::toggleSceneViewer(bool state) {
	_sceneViewer->setVisible(state);
}