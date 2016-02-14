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

class QtContext;


class ToolPropertyWidget : public QWidget {
	Q_OBJECT

public:

	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		parent		The optional parent
	 */
	ToolPropertyWidget(QWidget * parent = 0x0);

	/*!
	 *	@brief		Slot for being informed when a context gets activated.
	 *
	 *	@param		ctxId		The identifier for the activated context.
	 */
	void activated(size_t ctxId);

	/*!
	 *	@brief		Slot for being informed when a context gets deactivated
	 *
	 *	@param		ctxId		The context id for the context that has been
	 *							deactivated.
	 */
	void deactivated(size_t ctxId);

private:

	/*!
	 *	@brief		Queries the context manager for the given context.
	 *
	 *	@param		id		The id for the context to acquire.
	 *	@returns	The corresponding context, or null if none exists.
	 */
	QtContext * getContext(size_t id);

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