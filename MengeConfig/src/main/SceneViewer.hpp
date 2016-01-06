#ifndef __SCENE_VIEWER_H__
#define __SCENE_VIEWER_H__

#include <QtWidgets/qwidget.h>

QT_BEGIN_NAMESPACE
class QToolBar;
class QTextEdit;
QT_END_NAMESPACE

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
	 *	@brief		The tool bar for this window.
	 */
	QToolBar * _toolBar;

	/*!
	*	@brief		A child widget.
	*/
	QTextEdit * _someChild;
};

#endif	// __SCENE_VIEWER_H__