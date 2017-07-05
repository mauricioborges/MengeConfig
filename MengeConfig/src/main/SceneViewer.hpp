#ifndef __SCENE_VIEWER_H__
#define __SCENE_VIEWER_H__

#include <QtWidgets/qwidget.h>

QT_BEGIN_NAMESPACE
class QToolBar;
class QLabel;
class QComboBox;
QT_END_NAMESPACE

class GLWidget;

namespace Menge {
  namespace Agents {
    class SimulatorInterface;
  }
}

namespace MengeVis {
  namespace SceneGraph {
    class GLScene;
  }
  namespace Viewer {
    class ViewConfig;
  }
  namespace Runtime {
    class VisAgent;
  }
}

class SceneViewer : public QWidget {
	Q_OBJECT

public:
	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		parent		The optional parent 
	 */
	SceneViewer(QWidget * parent = 0x0);

  // TODO: This work should happen *outside* the scene viewer. A context should simply be applied
  // to the scene viewer.
	/*!
	 *	@brief		Starts the context for drawing obstacles.
	 */
	void drawObstacle();

  /*!
   *  @brief    Call back for when a new simulation gets loaded.
   *
   *  @param    sim     The simulation that got initialized; if null, clears the viewer.
   */
  void setSimulation( const Menge::Agents::SimulatorInterface* sim );

  /*!
   *  @brief    Configures the scene viewer bsaed on the given view configuration.
   *
   *  @param    view_config   The configuration for the viewer.
   */
  void setView( const MengeVis::Viewer::ViewConfig& view_config );

private:

	/*!
	 *	@brief		Updates the status text on the viewer.
	 */
	void updateStatus();

	/*!
	 *	@brief		Toggles whether or not the grid is drawn in the scene.
	 *				A grid that is not visible cannot be used for snapping.
	 *
	 *	@param		state		True to enable grid drawing, false to disable.
	 */
	void toggleGrid(bool state);

	/*!
	 *	@brief		Responds to users rotating the camera to aribtrary angles.
	 */
	void userRotated();

	/*!
	 *	@brief		Reports the current world position in the viewer.
	 *
	 *	@param		x		The x-value of the current position.
	 *	@param		y		The y-value of the current position.
	 */
	void setCurrentWorldPos(float x, float y);
	
	/*!
	 *	@brief		The tool bar for this window.
	 */
	QToolBar * _toolBar;

	/*!
	 *	@brief		The Open GL viewer.
	 */
	GLWidget * _glView;

	/*!
	 *	@brief		The combox box indicating camera axis orientation.
	 */
	QComboBox * _dirComboBox;

	/*!
	*	@brief		The status text for the viewer.
	*/
	QLabel * _statusLabel;

	/*!
	*	@brief		The current world position under the mouse
	*/
	QLabel * _posLabel;

	/*!
	 *	@brief		The action to toggle the grid's horizontal snap functiaonlity.
	 */
	QAction * _gridHSnap;

	/*!
	*	@brief		The action to toggle the grid's vertical snap functiaonlity.
	*/
	QAction * _gridVSnap;

  // Menge-specific data and methods

  // Clears the current scene.
  void clearScene();

  // Builds the scene from the current simulator -- clearing the current scene.
  void buildScene();

  // The current simulator.
  const Menge::Agents::SimulatorInterface* _sim{ nullptr };

  // The scene to draw
  MengeVis::SceneGraph::GLScene *	_scene{ nullptr };

  // The visual agents to draw.
  std::vector<MengeVis::Runtime::VisAgent*> _visAgents;

};

#endif	// __SCENE_VIEWER_H__