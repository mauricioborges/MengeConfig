/*!
 *	@file		QtContext.h
 *	@brief		The definition of a view context suitable for use with QT.
 */

#ifndef __QT_CONTEXT_H__
#define __QT_CONTEXT_H__

#include <SceneGraph/Context.h>
#include <SceneGraph/GLCamera.h>

#include "QtGui/qevent.h"

namespace Menge {
	namespace SceneGraph{
		class GLScene;
	}
}
QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE
class GLWidget;

/*!
 *	@brief		Context suitable for use with QT API.
 */
class QtContext : public Menge::SceneGraph::Context {
public:
	/*!
	 *	@brief		Default constructor.
	 */
	QtContext();

	/*!
	 *	@brief		Destructor.
	 */
	virtual ~QtContext();

	/*!
	 *	@brief		The draw function for the context.
	 *
	 *	@param		vWidth		The width of the viewport (in pixels).
	 *	@param		vHeight		The height of the viewport (in pixels).
	 */
	virtual void drawGL(int vWidth, int vHeight);

	/*!
	 *	@brief		Give the context the opportunity to respond to a mouse
	 *				event.
	 *
	 *	@param		e		The QT event with the mouse event data.
	 *	@param		view	The view this context is interacting with.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	virtual Menge::SceneGraph::ContextResult handleMouse(QMouseEvent * evt, GLWidget * view) { return Menge::SceneGraph::ContextResult(false, false); }

	/*!
	 *	@brief		Give the context the opportunity to respond to a keyboard
	 *				event.
	 *
	 *	@param		e		The QT event with the keyboard event data.
	 *	@param		view	The view this context is interacting with.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	virtual Menge::SceneGraph::ContextResult handleKeyboard(QKeyEvent * evt, GLWidget * view) { return Menge::SceneGraph::ContextResult(false, false); }

	/*!
	 *	@brief		Give the context the opportunity to respond to a mouse wheel
	 *				event.
	 *
	 *	@param		e		The QT event with the keyboard event data.
	 *	@param		view	The view this context is interacting with.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	virtual Menge::SceneGraph::ContextResult handleWheel(QWheelEvent * evt, GLWidget * view) { return Menge::SceneGraph::ContextResult(false, false); }

	/*!
	 *	@brief		Returns the control widget associated with this context.  
	 *
	 *				The main application can choose to populate a tool window with this widget.  It contains
	 *				buttons and settings to interact with or change state in the context.
	 *
	 *	@returns	A pointer to the tool widget for this context.
	 */
	virtual QWidget * getContextWidget() { return 0x0; }
};

/*!
 *	@brief		Selection context for performing selection in OpenGL view driven by QT.
 */
class QtSelectContext : public QtContext, public Menge::SceneGraph::SelectContext {
};

#endif	// __QT_CONTEXT_H__