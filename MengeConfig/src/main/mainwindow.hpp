#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include <MengeCore/Runtime/SimulatorDB.h>

class SceneHierarchy;
class SceneViewer;
class FSMViewer;
class AppLogger;
class ToolPropertyWidget;

/*!
 *	@brief		The main window of the application -- all GUI runs through this.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	/*!
	 *	@brief		Constructor.
	 */
  MainWindow();

private:
	
	///////////////////////////////////////////////////////////////////////////
	//				Gui Elements
	///////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The hierarchy visualizers -- the conceptual structure of the proejct is
	 *				drawn ehre..
	 */
	SceneHierarchy * _hierarchy;

	/*!
	 *	@brief		The dock widget containing the hierarchy widget -- used to handle visibility
	 *				control.
	 */
	QDockWidget * _hierarchyDock;

	/*!
	 *	@brief		The tool properties for the current context.
	 */
	ToolPropertyWidget * _toolProperties;

	/*!
	 *	@brief		The dock widget containing the tool properties -- used to handle visibility
	 *				control
	 */
	QDockWidget * _toolPropDock;

	/*!
	 *	@brief		The scene visualizers -- "phsycial" elements are drawn in this viewer.
	 */
	SceneViewer * _sceneViewer;

	/*!
	 *	@brief		The display of the finite state machine.
	 */
	FSMViewer * _fsmViewer;

	/*!
	 *	@brief		A reference to the logging window.
	 */
	AppLogger * _logger;

	///////////////////////////////////////////////////////////////////////////
	//				Menu items
	///////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Builds the main window.
	 */
	void buildMenu();

	/*!	@brief		Load new project action. */
	QAction *	_loadProject;

	/*! @brief		Reset project action. */
	QAction *	_resetProject;

	/*! @brief      Save project action. */	
	QAction *   _saveProject;

	/*!
	 *	@brief		Enters the mode for drawing obstacles.
	 */
	QAction *	_drawObstacleAct;

	/*!
	 *	@brief		The toggle for showing/hiding the scene viewer.
	 */
	QAction *	_toggleSceneVis;

	/*!
	 *	@brief		The toggle for showing/hiding the scene hierarchy.
	 */
	QAction *	_toggleSceneHierarchy;

	/*!
	 *	@brief		The toggle for showing/hiding the scene hierarchy.
	 */
	QAction *	_toggleToolProperties;

	/*!
	 *	@brief		The toggle for showing/hiding the behavior fsm viewer.
	 */
	QAction *	_toggleFsmVis;

	/*!
	 *	@brief		The toggle for showing/hiding the logger window.
	 */
	QAction *	_toggleLogVis;

	/*!
	 *	@brief		Causes the hierarchy visibility to be toggled.
	 *
	 *	@param		state		The visibility to set on the hierarchy.
	 */
	void toggleHierarchy(bool state);

	/*!
	 *	@brief		Informs the app that the hierarchy has changed.
	 *
	 *	@param		state		The visibility state of the widget.
	 */
	void hierarchyVisibilityChanged(bool state);

	/*!
	 *	@brief		Causes the tool property visibility to be toggled.
	 *
	 *	@param		state		The visibility to be set on the window.
	 */
	void toggleToolProperties(bool state);

	/*!
	*	@brief		Informs the app that the tool property window has changed.
	*
	*	@param		state		The visibility state of the widget.
	*/
	void toolPropertiesVisibilityChanged(bool state);

	/*!
	 *	@brief		Slot for when the state of the scene viewer visibility changes.
	 *
	 *	@param		state		The visibility state of the widget.
	 */
	void toggleSceneViewer(bool state);

	/*!
	*	@brief		Slot for when the state of the fsm viewer visibility changes.
	*
	*	@param		state		The visibility state of the widget.
	*/
	void toggleFSMViewer(bool state);

	/*!
	*	@brief		Slot for when the state of the log widget changes.
	*
	*	@param		state		The visibility state of the widget.
	*/
  void toggleLog( bool state );

  /*!
  *	@brief		Attempts to load a Menge project.
  */
  void loadProject();

  /*!
  *	@brief		Resets the project to its initial state.
  */
  void resetProject();

  /*!
  *	@brief		Attempts to save the current project into Menge files.
  */
  void saveProject();

  ///////////////////////////////////////////////////////////////////////////
  //				Menge Data Structures
  ///////////////////////////////////////////////////////////////////////////

  Menge::SimulatorDB simDB_;

};

#endif
