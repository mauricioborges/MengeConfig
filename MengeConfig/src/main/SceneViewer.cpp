#include "SceneViewer.hpp"

#include "AppLogger.hpp"
#include "ContextManager.hpp"
#include "glwidget.hpp"
#include "ObstacleContext.hpp"
#include "player_controller.hpp"

#include <set>

#include <QtWidgets/qaction.h>
#include <QtWidgets/QBoxLayout.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/QLabel.h>
#include <QtWidgets/QMenu.h>
#include <QtWidgets/qToolbar.h>

#include <MengeCore/Agents/BaseAgent.h>
#include <MengeCore/Agents/Obstacle.h>
#include <MengeCore/Agents/SimulatorInterface.h>
#include <MengeCore/Agents/SpatialQueries/SpatialQuery.h>
#include <MengeCore/Math/vector.h>
#include <MengeVis/Runtime/VisAgent/VisAgent.h>
#include <MengeVis/Runtime/VisObstacle.h>
#include <MengeVis/SceneGraph/GLScene.h>

using Menge::Agents::BaseAgent;
using Menge::Agents::Obstacle;
using Menge::Agents::SimulatorInterface;
using Menge::Agents::SpatialQuery;
using Menge::Math::Vector2;
using Menge::Math::Vector3;
using MengeVis::Runtime::VisAgent;
using MengeVis::Runtime::VisObstacle;
using MengeVis::SceneGraph::GLScene;

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of SceneViewer
/////////////////////////////////////////////////////////////////////////////////////////////

SceneViewer::SceneViewer(QWidget * parent) : QWidget(parent) {
	QVBoxLayout * mainLayout = new QVBoxLayout();

  _contextMenu = new QMenu( tr( "Context Menu" ), this );
  QMenu* grid_menu = _contextMenu->addMenu( tr( "Grid" ) );
  _cameraContextMenu = _contextMenu->addMenu( "Cameras" );

	_toolBar = new QToolBar();
	mainLayout->addWidget(_toolBar, 0);
	mainLayout->setMargin(0);

	_glView = new GLWidget();
	mainLayout->addWidget(_glView, 1);

  _player_controller = new PlayerController();
  mainLayout->addWidget( _player_controller, 0 );
  connect( _player_controller, &PlayerController::needRedraw, this, &SceneViewer::updateForRedraw );

	_statusLabel = new QLabel("Scene Viewer:");
	QHBoxLayout * statusLayout = new QHBoxLayout();
	statusLayout->setContentsMargins(5, 1, 5, 1);
	statusLayout->addWidget(_statusLabel, 1);
  
	_posLabel = new QLabel("no point");
	statusLayout->addWidget(_posLabel, 0, Qt::AlignRight);

  this->setContextMenuPolicy( Qt::CustomContextMenu );
  connect( this, &SceneViewer::customContextMenuRequested, this, &SceneViewer::showContextMenu );

	mainLayout->addLayout(statusLayout, 0);

	// Populate tool bar
	QAction * togAxisAct = new QAction(QIcon(":/images/toggleAxis.png"), tr("Toggle &Axis"), this);
	togAxisAct->setCheckable(true);
	togAxisAct->setChecked(true);
	togAxisAct->setToolTip(tr("Toggle the display of the axis in the scene viewer"));
	_toolBar->addAction(togAxisAct);
	connect(togAxisAct, &QAction::triggered, _glView, &GLWidget::setDrawAxis);
  _contextMenu->addAction( togAxisAct );

	QAction * togPerspAct = new QAction(QIcon(":/images/togglePersp.png"),
                                      tr("Toggle &Perspective"), this);
	togPerspAct->setCheckable(true);
	togPerspAct->setChecked(true);
	togPerspAct->setToolTip(
    tr("Toggle the current camera's projection between perspective and orthographic"));
	_toolBar->addAction(togPerspAct);
	connect(togPerspAct, &QAction::triggered, _glView, &GLWidget::toggleProjection);
  connect( _glView, &GLWidget::setViewPerspective, togPerspAct, &QAction::setChecked );

	_dirComboBox = new QComboBox();
	_dirComboBox->setEditable(false);
	QStringList directions;
	directions << "user-defined" << "x" << "-x" << "y" << "-y" << "z" << "-z";
	_dirComboBox->addItems(directions);
	_dirComboBox->setCurrentIndex(5);
	_toolBar->addSeparator();
	_toolBar->addWidget(_dirComboBox);
	_toolBar->addSeparator();
	// TODO: Figure out why the function pointer version didn't work and I had to use the old
  // slot/signal crap. 
  // connect(dirComboBox, &QComboBox::currentIndexChanged, _glView, &GLWidget::setViewDirection);
	connect(_dirComboBox, SIGNAL(activated(int)), _glView, SLOT(setViewDirection(int)));

	QAction * togGridAct = new QAction(QIcon(":/images/toggleGrid.png"), tr("Toggle &Grid"), this);
	togGridAct->setCheckable(true);
	togGridAct->setChecked(true);
	togGridAct->setToolTip(
    tr("Toggle the reference grid; an inactive grid cannot be used for snapping."));
	_toolBar->addAction(togGridAct);
	connect(togGridAct, &QAction::triggered, this, &SceneViewer::toggleGrid);
  grid_menu->addAction( togGridAct );

	QAction * gridPropAct = new QAction(QIcon(":/images/gridProperties.png"),
                                      tr("Grid &Properties"), this);
	gridPropAct->setToolTip(tr("Edit the reference grid's properties."));
	_toolBar->addAction(gridPropAct);
	connect(gridPropAct, &QAction::triggered, _glView, &GLWidget::editGridProperties);
  grid_menu->addAction( gridPropAct );

	_gridHSnap = new QAction(QIcon(":/images/gridSnapHorizontal.png"), tr("&Horizontal Snap"), this);
	_gridHSnap->setCheckable(true);
	_gridHSnap->setChecked(false);
	_gridHSnap->setToolTip(tr("Causes mouse selection points to snap to horizontal grid lines."));
	_toolBar->addAction(_gridHSnap);
	connect(_gridHSnap, &QAction::triggered, _glView, &GLWidget::toggleHorizontalSnap);
  grid_menu->addAction( _gridHSnap );

	_gridVSnap = new QAction(QIcon(":/images/gridSnapVertical.png"), tr("&Vertical Snap"), this);
	_gridVSnap->setCheckable(true);
	_gridVSnap->setChecked(false);
	_gridVSnap->setToolTip(tr("Causes mouse selection points to snap to vertical grid lines."));
	_toolBar->addAction(_gridVSnap);
	connect(_gridVSnap, &QAction::triggered, _glView, &GLWidget::toggleVerticalSnap);
  grid_menu->addAction( _gridVSnap );

	connect(_glView, &GLWidget::userRotated, this, &SceneViewer::userRotated);
	connect(_glView, &GLWidget::currWorldPos, this, &SceneViewer::setCurrentWorldPos);

	setLayout(mainLayout);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::drawObstacle() {
  // TODO: Determine where the context comes from.
  QtContext * ctx = new ObstacleContext();
  ContextManager::instance()->activate( ctx );
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::setSimulation( Menge::Agents::SimulatorInterface* sim ) {
  _sim = sim;
  buildScene();
  _player_controller->setSimulation( sim );
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::setView( const MengeVis::Viewer::ViewConfig& view_config ) {
  _glView->setView( view_config );

  _cameraContextMenu->clear();
  QActionGroup* group = new QActionGroup( this );
  for ( size_t c = 0; c < _glView->get_num_cameras(); ++c ) {
    QAction* action = new QAction( QString::fromStdString( _glView->get_camera_name( c ) ), this );
    action->setCheckable( true );
    action->setChecked( c == 0 );
    action->setActionGroup( group );
    connect( action, &QAction::triggered,
             this, [ this, c ]( bool checked ) { this->_glView->set_camera( c ); } );
    _cameraContextMenu->addAction( action );
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::toggleGrid(bool state) {
	_glView->toggleReferenceGrid(state);
	_gridHSnap->setEnabled(state);
	_gridVSnap->setEnabled(state);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::updateForRedraw() {
  for ( size_t i = 0; i < _visAgents.size(); ++i ) {
    VisAgent* vis_agent = _visAgents[ i ];
    const BaseAgent * agt = vis_agent->getAgent();
    float h = _sim->getElevation( agt );
    vis_agent->setPosition( agt->_pos.x(), agt->_pos.y(), h );
  }
  _glView->update();
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

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::showContextMenu( const QPoint& pos ) {
  _contextMenu->exec( mapToGlobal( pos ) );
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::clearScene() {
  if ( _scene != nullptr ) {
    // TODO: Do the work to flush all of the scene elements.
    _glView->setScene( _scene );
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void SceneViewer::buildScene() {
  clearScene();
  if ( _sim != nullptr ) {
    // TODO: Do the work to populate the scene.
    _scene = new GLScene();
    // Add agents.
    _visAgents.reserve( _sim->getNumAgents() );
    for ( size_t a = 0; a < _sim->getNumAgents(); ++a ) {
      const BaseAgent * agt = _sim->getAgent( a );
      VisAgent * agtNode = new VisAgent();
      agtNode->setElement( agt );
      float h = _sim->getElevation( agt );
      agtNode->setPosition( agt->_pos.x(), agt->_pos.y(), h );
      _scene->addNode( agtNode );
      _visAgents.push_back(agtNode);
    }
    // Add obstacles.
    // TODO: If the bsptree (ObstacleKDTree.h) chops up the obstacles, this isn't doing the
    //		right thing.  Currently, the bsptree chops them
    //	THIS IS A HACK to address the issues of the ObstacleKDTree
    //		The right thing to do is modify things so that they are not chopped up.
    std::set< const Obstacle * > handled;
    const std::vector< Obstacle * > & obstacles = _sim->getSpatialQuery().getObstacles();
    for ( size_t o = 0; o < obstacles.size(); ++o ) {

      const Obstacle * obst = obstacles[ o ];
      if ( handled.find( obst ) == handled.end() ) {
        Vector2 p0a = obst->getP0();
        Vector2 p1a = obst->getP1();
        const Obstacle * next = obst->_nextObstacle;
        while ( next && next->_unitDir * obst->_unitDir >= 0.99999f ) {
          handled.insert( next );
          p1a.set( next->getP1() );
          next = next->_nextObstacle;
        }
        Vector3 p0( p0a.x(), p0a.y(), _sim->getElevation( p0a ) );
        Vector3 p1( p1a.x(), p1a.y(), _sim->getElevation( p1a ) );
        VisObstacle * vo = new VisObstacle( p0, p1 );

        _scene->addNode( vo );
        handled.insert( obst );
      }
    }
    _glView->setScene( _scene );
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////