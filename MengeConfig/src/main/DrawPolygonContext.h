/*!
 *	@file		DrawPolygonContext.h
 *	@brief		The definition of a context for drawing polygons.
 */

#include "QtContext.h"

// Forward declarations
class GLPolygon;
class DrawPolygonWidget;

/*!
 *	@brief		A functor for serving as the call back fo the DrawPolygonContext
 *				when a polygon has been created.
 */
class PolygonCreatedCB {
public:

	/*!
	 *	@brief		The callback when a polygon has been created.
	 *				
	 *	It is the responsibility of the functor to take posession (or serve
	 *	as the means of transmission) of the polygon and then report on success.  
	 *	
	 *	@param		poly		The new polygon made available.
	 *	@returns	True if ownership of the polygon is taken, false if not.
	 *				If false is returned, the polygon will be destroyed.
	 */
	virtual bool newPolygon(GLPolygon * poly) = 0;
};

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
	virtual MengeVis::SceneGraph::ContextResult handleMouse(QMouseEvent * evt, GLWidget * view);

	/*!
	 *	@brief		Give the context the opportunity to respond to a keyboard
	 *				event.
	 *
	 *	@param		e		The QT event with the keyboard event data.
	 *	@param		view	The view this context is interacting with.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	virtual MengeVis::SceneGraph::ContextResult handleKeyboard(QKeyEvent * evt, GLWidget * view);

	/*!
	*	@brief		Returns the control widget associated with this context.
	*
	*				The main application can choose to populate a tool window with this widget.  It contains
	*				buttons and settings to interact with or change state in the context.
	*
	*	@returns	A pointer to the tool widget for this context.
	*/
	virtual QWidget * getContextWidget();

	/*!
	 *	@brief		Give the context the opportunity to respond to a mouse wheel
	 *				event.
	 *
	 *	@param		e		The QT event with the keyboard event data.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	//virtual MengeVis::SceneGraph::ContextResult handleWheel(QWheelEvent * evt);

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

	/*!
	 *	@brief		Registers the callback functor to the context.
	 *
	 *	If the context already has a context, an error will be thrown.  The context
	 *	will not allow the callback to be "hijacked" (under normal circumstances.)
	 *	Although, this behavior can be forced.
	 *
	 *	@param		callback		A pointer to the callback.  The context does not
	 *								take ownership of this.
	 *	@param		force			If true, the callback will be set, even if there
	 *								is already a registered callback.  False will
	 *								cause an exception if already set.
	 *	@throws		MCException thrown if the context already has a callback (and force is false).
	 */
	void registerNewPolyCallback(PolygonCreatedCB * callack, bool force = false);

	/*!
	 *	@brief		Removes the given callback from the context -- if it is
	 *				registered.
	 *
	 *	@brief		callback		The callback to unregister.
	 *	@returns	Indicates whether the callback was successfully unregistered.
	 *				true if the given callback was previously registered.  False if not
	 */
	bool unregisterNewPolyCallback(PolygonCreatedCB * callback);

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
	 *	@brief		Indicates that a point is being drawn and moved at the same time.
	 */
	bool _dragging;

	/*!
	 *	@brief		The callback for polygon creation.
	 */
	PolygonCreatedCB * _newPolyCB;

	/*!
	 *	@brief		The underlying widget associated with this context.
	 */
	DrawPolygonWidget * _widget;
};
