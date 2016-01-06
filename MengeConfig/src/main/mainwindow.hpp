
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class Window;
class SceneHierarchy;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
	Window * _window;
	SceneHierarchy * _hierarchy;
	QDockWidget * _hierarchyDock;

	///////////////////////////////////////////////////////////////////////////
	//				Menu items
	///////////////////////////////////////////////////////////////////////////

	QAction *	_toggleSceneVis;
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

//private slots:
    //void onAddNew();
};

#endif
