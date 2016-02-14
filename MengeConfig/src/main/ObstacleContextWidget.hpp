/*!
 *	@file		ObstacleContextWidget.hpp
 *	@brief		The definition of the widget that provides a UI to the obstacle context.
 */

#ifndef __OBSTACLE_CONTEXT_WIDGET_H__
#define	__OBSTACLE_CONTEXT_WIDGET_H__

#include <QtWidgets\qwidget.h>


QT_BEGIN_NAMESPACE
class QAction;
class QTabWidget;

QT_END_NAMESPACE

class ObstacleContext;
class EditPolygonWidget;
class DrawPolygonWidget;

class ObstacleContextWidget : public QWidget {
	Q_OBJECT

public:
	/*! 
	 *	@brief		constructor.
	 *
	 *	@param		context			The context this widget interfaces with.
	 *	@param		editWidget		The edit polygon context widget.
	 *	@param		drawWidget		The edit polygon context widget.
	 *	@param		parent			The optional parent widget.
	 */
	ObstacleContextWidget(ObstacleContext * context, EditPolygonWidget * editWidget, DrawPolygonWidget * drawWidget, QWidget * parent = 0x0);

	/*!
	 *	@brief		Destructor
	 */
	~ObstacleContextWidget();

protected:

	/*!
	 *	@brief		Changes the obstacle editing state based on tab selection.
	 *
	 *	@param		int		The index of the current tab.
	 */
	void tabChanged(int index);

	/*!
	 *	@brief		The obstacle context.
	 */
	ObstacleContext * _context;

	/*!
	 *	@brief		The edit polygon context widget.
	 */
	EditPolygonWidget * _editWidget;

	/*!
	 *	@brief		The draw polygon context widget.
	 */
	DrawPolygonWidget * _drawWidget;

	/*!
	 *	@brief		The widget that contains the edit/draw widgets.
	 */
	QTabWidget * _tabWidget;
};

#endif	// __OBSTACLE_CONTEXT_WIDGET_H__