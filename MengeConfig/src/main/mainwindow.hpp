
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class SceneHierarchy;
class SceneViewer;
class FSMViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
	
	SceneHierarchy * _hierarchy;
	QDockWidget * _hierarchyDock;
	SceneViewer * _sceneViewer;
	FSMViewer * _fsmViewer;
	//QSplitter *splitter;

	///////////////////////////////////////////////////////////////////////////
	//				Menu items
	///////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The toggle for showing/hiding the scene viewer.
	 */
	QAction *	_toggleSceneVis;

	/*!
	 *	@brief		The toggle for shoing/hiding the scene hierarchy.
	 */
	QAction *	_toggleSceneHierarchy;

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
	* Slot for when the state of the log visibility changes.
	*/
	void toggleSceneViewer(bool state);

};

#endif
