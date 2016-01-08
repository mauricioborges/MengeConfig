#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class SceneHierarchy;
class SceneViewer;
class FSMViewer;

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
	 *	@brief		The scene visualizers -- "phsycial" elements are drawn in this viewer.
	 */
	SceneViewer * _sceneViewer;

	/*!
	 *	@brief		The display of the finite state machine.
	 */
	FSMViewer * _fsmViewer;

	///////////////////////////////////////////////////////////////////////////
	//				Menu items
	///////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The toggle for showing/hiding the scene viewer.
	 */
	QAction *	_toggleSceneVis;

	/*!
	 *	@brief		The toggle for showing/hiding the scene hierarchy.
	 */
	QAction *	_toggleSceneHierarchy;

	/*!
	 *	@brief		The toggle for showing/hiding the behavior fsm viewer.
	 */
	QAction *	_toggleFsmVis;

	/*!
	 *	@brief		Causes the hierarchy visibility to be toggled.
	 *
	 *	@param		state The visibility to set on the hierarchy.
	 */
	void toggleHierarchy(bool state);

	/*!
	 *	@brief		Informs the app that the hierarchy has changed.
	 *
	 *	@param		state		The visibility state of the widget.
	 */
	void hierarchyVisibilityChanged(bool state);

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

};

#endif
