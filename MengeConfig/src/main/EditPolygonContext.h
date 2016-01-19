/*!
 *	@file		EditPolygonContext.h
 *	@brief		The definition of a context for editing polygons.
 */

#ifndef __EDIT_POLYGON_CONTEXT_H__
#define __EDIT_POLYGON_CONTEXT_H__

#include "QtCOntext.h"
#include "LiveObstacleSet.h"

// forward declarations
class GLPolygon;

/*!
 *	@brief		Context for editing existing polygons.
 */
class EditPolygonContext : public QtContext {

	/*!
	 *	@brief		Indicator of what polygon element is being manipulated.
	 */
	enum EditMode {
		NO_EDIT,		///< Nothing is being manipulated.
		VERTEX,			///< Editing vertices
		EDGE,			///< Editing edges
		POLY			///< Editing polygons
	};

public:


	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		polygons		The set of polygons to edit.
	 */
	EditPolygonContext(LiveObstacleSet * polygons);

	/*!
	 *	@brief		Destructor.
	 */
	virtual ~EditPolygonContext();

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
	 *	@brief		Called when the context is activated.
	 */
	virtual void activate();

	/*!
	 *	@brief		Called when the context is deactivated.
	 */
	virtual void deactivate();


protected:

	/*!
	*	@brief		Draw context elements into the 3D world.
	*
	*	@param		select		Defines if the drawing is being done for selection
	*							purposes (true) or visualization (false).
	*/
	virtual void draw3DGL(bool select = false);

	/*!
	*	@brief		Sets the editing mode of the context.
	*/
	void setEditMode(EditMode mode);



	/*!
	 *	@brief		The set of polygons to edit.  The class does *not* own this obstacle set.
	 */
	LiveObstacleSet	* _obstacleSet;

	/*!
	 *	@brief		The polygon currently being edited.  The class does *not* own this polygon.
	 */
	GLPolygon * _activePoly;

	/*!
	 *	@brief		The active vertex.
	 */
	SelectVertex _activeVert;

	/*!
	 *	@brief		Indicates if a feature is currently being moved (true).
	 */
	bool _dragging;

	/*!
	 *	@brief		The editing mode -- what type of feature is being modified.
	 */
	EditMode _mode;

	/*!
	 *	@brief		The world space position of the mouse (on the ground plane) when the
	 *				mouse button was pressed.
	 */
	Vector2 _downPos;

	/*!
	 *	@brief		The origin of transformation when the mouse was pressed.
	 */
	Vector2 _downOrigin;

	// _edgeDir
	// _activeId
};

#endif	// __EDIT_POLYGON_CONTEXT_H__
