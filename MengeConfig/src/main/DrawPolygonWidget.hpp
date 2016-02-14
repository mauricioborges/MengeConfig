/*!
 *	@file		DrawPolygonWidget.hpp
 *	@brief		The widget for interacting with the draw polygon behavior.
 */

#ifndef __DRAW_POLYGON_WIDGET_H__
#define	__DRAW_POLYGON_WIDGET_H__


#include <QtWidgets\qwidget.h>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class DrawPolygonContext;

class DrawPolygonWidget : public QWidget {
	Q_OBJECT

public:
	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		context			The context this widget interfaces with.
	 *	@param		parent			The optional parent widget.
	 */
	DrawPolygonWidget(DrawPolygonContext * context, QWidget * parent = 0x0);

protected:

	/*!
	*	@brief		The obstacle context.
	*/
	DrawPolygonContext * _context;
};


#endif	// __DRAW_POLYGON_WIDGET_H__