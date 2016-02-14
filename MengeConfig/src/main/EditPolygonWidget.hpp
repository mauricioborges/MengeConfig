/*!
*	@file		EditPolygonWidget.hpp
*	@brief		The widget for interacting with the edit polygon behavior.
*/

#ifndef __EDIT_POLYGON_WIDGET_H__
#define	__EDIT_POLYGON_WIDGET_H__


#include <QtWidgets\qwidget.h>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class EditPolygonContext;

class EditPolygonWidget : public QWidget {
	Q_OBJECT

public:
	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		context			The context this widget interfaces with.
	 *	@param		parent			The optional parent widget.
	 */
	EditPolygonWidget(EditPolygonContext * context, QWidget * parent = 0x0);


protected:

	/*!
	 *	@brief		The obstacle context.
	 */
	EditPolygonContext * _context;
};


#endif	// __EDIT_POLYGON_WIDGET_H__