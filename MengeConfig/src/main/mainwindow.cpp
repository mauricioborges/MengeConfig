#include "mainwindow.hpp"

#include "AppLogger.hpp"
#include "ContextManager.hpp"
#include "FSMViewer.hpp"
#include "PedestrianModelDialog.hpp"
#include "SceneHierarchy.hpp"
#include "SceneViewer.hpp"
#include "SceneHierarchy.hpp"
#include "ToolProperties.hpp"

#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/QDockWidget.h>
#include <QtWidgets/QFileDialog.h>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar.h>

#include <MengeCore/PluginEngine/CorePluginEngine.h>

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of MainWindow
/////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow() : QMainWindow()
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
	
  this->statusBar()->showMessage( tr( "Starting..." ), 2000 );

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

  Menge::PluginEngine::CorePluginEngine pluginEngine( &simDB_ );
  AppLogger::logStream << AppLogger::INFO_MSG << "Menge has " << simDB_.modelCount() << " models "
    << "available." << AppLogger::END_MSG;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::buildMenu() {
	QMenuBar *menuBar = new QMenuBar;

	// Projects menu
	QMenu * menuProj = menuBar->addMenu( tr( "&Project" ) );
	_loadProject = new QAction( menuProj );
	_loadProject->setText( tr( "Open Project" ) );
  _loadProject->setShortcut( QKeySequence( tr( "Ctrl+o" ) ) );
  _loadProject->setStatusTip( tr( "Open a Menge simulation project from disk" ) );
	_loadProject->setEnabled( true );
	menuProj->addAction( _loadProject );
  connect( _loadProject, &QAction::triggered, this, &MainWindow::loadProject );

	_resetProject = new QAction( menuProj );
  _resetProject->setText( tr( "Reset Project" ) );
  _resetProject->setShortcut( QKeySequence( tr( "Ctrl+r" ) ) );
  _resetProject->setStatusTip( tr( "Reset the project to its on-disk state" ) );
	_resetProject->setEnabled( false );
	menuProj->addAction( _resetProject );
  connect( _resetProject, &QAction::triggered, this, &MainWindow::resetProject );

	_saveProject = new QAction( menuProj );
  _saveProject->setText( tr( "Save Project" ) );
  _saveProject->setShortcut( QKeySequence( tr( "Ctrl+Shift+S" ) ) );
  _saveProject->setStatusTip( tr( "Save the current project to disk" ) );
	_saveProject->setEnabled( false );
	menuProj->addAction( _saveProject );
  connect( _saveProject, &QAction::triggered, this, &MainWindow::saveProject );

	// Obstacles menu
	QMenu * menuObst = menuBar->addMenu(tr("&Obstacles"));

	_drawObstacleAct = new QAction(menuObst);
	_drawObstacleAct->setText(tr("&Draw Obstacle"));
  _drawObstacleAct->setStatusTip( tr( "Begin drawing obstacles in the simulation domain" ) );
	menuObst->addAction(_drawObstacleAct);
	connect(_drawObstacleAct, &QAction::triggered, _sceneViewer, &SceneViewer::drawObstacle);


	// View menu
	QMenu *menuView = menuBar->addMenu(tr("&View"));

	_toggleSceneVis = new QAction(menuView);
	_toggleSceneVis->setText(tr("Scene Viewer"));
  _toggleSceneVis->setStatusTip( tr( "Toggle the display of the main scene" ) );
	_toggleSceneVis->setCheckable(true);
	_toggleSceneVis->setChecked(true);
	menuView->addAction(_toggleSceneVis);
	connect(_toggleSceneVis, &QAction::triggered, this, &MainWindow::toggleSceneViewer);

	_toggleSceneHierarchy = new QAction(menuView);
	_toggleSceneHierarchy->setText(tr("Scene Hierarchy"));
  _toggleSceneHierarchy->setStatusTip( tr( "Toggle the display of the scene hierarchy" ) );
	_toggleSceneHierarchy->setCheckable(true);
	_toggleSceneHierarchy->setChecked(true);
	menuView->addAction(_toggleSceneHierarchy);
	connect(_toggleSceneHierarchy, &QAction::triggered, this, &MainWindow::toggleHierarchy);

	_toggleFsmVis = new QAction(menuView);
	_toggleFsmVis->setText(tr("FSM Viewer"));
  _toggleFsmVis->setStatusTip( 
    tr( "Toggle the display of the Behavior Finite State Machine viewer" ) );
	_toggleFsmVis->setCheckable(true);
	_toggleFsmVis->setChecked(true);
	menuView->addAction(_toggleFsmVis);
	connect(_toggleFsmVis, &QAction::triggered, this, &MainWindow::toggleFSMViewer);

	_toggleLogVis = new QAction(menuView);
	_toggleLogVis->setText(tr("Activity Log"));
  _toggleLogVis->setStatusTip( tr( "Toggle display of the work log" ) );
	_toggleLogVis->setCheckable(true);
	_toggleLogVis->setChecked(false);
	menuView->addAction(_toggleLogVis);
	connect(_toggleLogVis, &QAction::triggered, this, &MainWindow::toggleLog);

	_toggleToolProperties = new QAction(menuView);
	_toggleToolProperties->setText(tr("Tool Properties"));
  _toggleToolProperties->setStatusTip( tr( "Toggle the display of the current tool's properties" ) );
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

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::loadProject() {
  QFileDialog dialog( this );
  dialog.setWindowTitle( "Open Project" );
  dialog.setFileMode( QFileDialog::ExistingFile );
  dialog.setNameFilter( tr( "Project files (*.xml);;All files(*.*)" ) );
  
  if ( dialog.exec() ) {
    QStringList fileNames = dialog.selectedFiles();
    std::string fileName = fileNames[0].toStdString();
    AppLogger::logStream << AppLogger::INFO_MSG << "Selected " << fileName
      << " to load" << AppLogger::END_MSG;
    // Load the project
    //  1. Select pedestrian model
    PedestrianModelDialog model_dlg( &simDB_, this );
    if ( model_dlg.exec() ) {
      //  2. Parse project file (having already set the model in the ProjectSpec).
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::resetProject() {
  AppLogger::logStream << AppLogger::INFO_MSG << "Reset project not implemented yet..."
    << AppLogger::END_MSG;
}

/////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::saveProject() {
  AppLogger::logStream << AppLogger::INFO_MSG << "Save project not implemented yet..."
    << AppLogger::END_MSG;
}