#ifndef __SCENE_HIERARCHY_H__
#define __SCENE_HIERARCHY_H__

#include <QtWidgets/qwidget.h>

QT_BEGIN_NAMESPACE
class QToolBar;
class QTreeWidget;
QT_END_NAMESPACE

class SceneHierarchy : public QWidget {
	Q_OBJECT

public:
	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		parent		The optional parent
	 */
	SceneHierarchy(QWidget * parent = 0x0);

private:

	/*!
	 *	@brief		The tool bar for this window.
	 */
	QToolBar * _toolBar;

	/*!
	 *	@brief		A child widget.
	 */
	QTreeWidget * _sceneTree;
};

#endif	// __SCENE_HIERARCHY_H__