/*! 
 *	@file		ToolProperties.hpp
 *	@brief		The definition of the tool properties widget -- it displays specific
 *				context properties for the active context.
 */

#ifndef __TOOL_PROPERTIES_H__
#define __TOOL_PROPERTIES_H__

#include <QtWidgets/qwidget.h>


QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE


class ToolPropertyWidget : public QWidget {
	Q_OBJECT

public:

	/*!
	*	@brief		Constructor.
	*
	*	@param		parent		The optional parent
	*/
	ToolPropertyWidget(QWidget * parent = 0x0);

private:

	/*!
	 *	@brief		The main context widget displayed for the current context.
	 */
	QWidget * _ctxWidget;

	/*!
	 *	@brief		The default widget when there is no context widget.
	 */
	QWidget * _defaultWidget;

	/*!
	 *	@brief		The layout for the widget.
	 */
	QVBoxLayout * _layout;
};

#endif	// __TOOL_PROPERTIES_H__