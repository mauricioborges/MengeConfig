/*!
*	@file		DrawPolygonContext.h
*	@brief		The definition of a context for drawing polygons.
*/

#include "QtContext.h"

// Forward declarations
class GLPolygon;

/*!
 *	@brief		Provides a context for drawing polygons.
 */
class DrawPolygonContext : public QtContext {
	/*!
	 *	@brief		Definition of the context's drawing state.
	 */
	enum DrawStateEnum {
		WAITING,
		DRAWING
	};

public:

	/*!
	 *	@brief		Constructor.
	 */
	DrawPolygonContext();

	/*!
	 *	@brief		Destructor.
	 */
	~DrawPolygonContext();

	/*!
	 *	@brief		Give the context the opportunity to respond to a mouse
	 *				event.
	 *
	 *	@param		e		The QT event with the mouse event data.
	 *	@param		view	The view this context is interacting with.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	virtual Menge::SceneGraph::ContextResult handleMouse(QMouseEvent * evt, GLWidget * view);

	/*!
	 *	@brief		Give the context the opportunity to respond to a keyboard
	 *				event.
	 *
	 *	@param		e		The QT event with the keyboard event data.
	 *	@param		view	The view this context is interacting with.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	virtual Menge::SceneGraph::ContextResult handleKeyboard(QKeyEvent * evt, GLWidget * view);

	/*!
	 *	@brief		Give the context the opportunity to respond to a mouse wheel
	 *				event.
	 *
	 *	@param		e		The QT event with the keyboard event data.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	//virtual Menge::SceneGraph::ContextResult handleWheel(QWheelEvent * evt);

	/*!
	 *	@brief		Reverses the winding of the active polygon.
	 *
	 *	@returns	True if the action requires a redraw, false otherwise.
	 */
	bool reverseWinding();

	/*!
	 *	@brief		Remvoes the last vertex from the polygon -- essentially undoing the last.
	 *
	 *	@returns	True if the action requires a redraw, false otherwise.
	 */
	bool popVertex();
	 
	/*!
	 *	@brief		Called when the context is activated.
	 */
	virtual void activate();

	/*!
	 *	@brief		Called when the context is deactivated.
	 */
	virtual void deactivate();

	/*!
	 *	@brief		Deletes the active polygon.
	 *
	 *	@returns	True if the action requires a redraw, false otherwise.
	 */
	bool deleteActive();

protected:
	/*!
	 *	@brief		Draw context elements into the 3D world.
	 *
	 *	@param		select		Defines if the drawing is being done for selection
	 *							purposes (true) or visualization (false).
	 */
	virtual void draw3DGL(bool select = false);

private:
	/*!
	 *	@brief		The state of the context.
	 */
	DrawStateEnum _state;

	/*!
	 *	@brief		Active polygon.
	 */
	GLPolygon * _polygon;

	/*!
	 *	@breif		Indicates that a point is being drawn and moved at the same time.
	 */
	bool _dragging;
};
