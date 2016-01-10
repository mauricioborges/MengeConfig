#ifndef __SCENE_VIEWER_H__
#define __SCENE_VIEWER_H__

#include <QtWidgets/qwidget.h>

QT_BEGIN_NAMESPACE
class QToolBar;
class QLabel;
class QComboBox;
QT_END_NAMESPACE
class GLWidget;

class SceneViewer : public QWidget {
	Q_OBJECT

public:
	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		parent		The optional parent 
	 */
	SceneViewer(QWidget * parent = 0x0);

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

};

#endif	// __SCENE_VIEWER_H__