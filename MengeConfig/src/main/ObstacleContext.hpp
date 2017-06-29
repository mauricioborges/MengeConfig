/*!
 *	@file		DrawObstacleContext.h
 *	@brief		This context is responsible for the logic for editing obstacle
 *				sets.
 */

#ifndef __DRAW_OBSTACLE_CONTEXT_H__
#define __DRAW_OBSTACLE_CONTEXT_H__

#include "QtContext.h"
#include "QtCore/qobject"

namespace Menge {
	namespace Agents {
		class ExplicitObstacleSet;
		class ObstacleSet;
	}
}
class LiveObstacleSet;
class PolygonCreatedCB;
class ObstacleContextWidget;

// TODO:
//	- Create new obstacle set
//		- Broadcast the pointer to be added to the underlying scene structure, gl scene, hierarchy view.
//	- Create modes
//		- new obstacle
//			- basically draw a polygon
//		- edit obstacle
//			- change points in polygon
//				- insert
//				- move
//				- delete
//	- Create tool widget
//		- Reverse obstacle direction
//	...
class ObstacleContext : public QObject, public QtContext {

	Q_OBJECT

public:

	/*!
	 *	@brief		The type of action being taken w.r.t. the obstacle set.
	 *
	 *	Designed to serve as indices into the set of child contexts that
	 *	actually perform the work.
	 */
	enum ObstacleVerb {
		NEW_OBSTACLE = 0,		// create a new obstacle
		EDIT_OBSTACLE,			// edit existing obstacles
		NONE,					// no action taken on obstacles
	};

	/*!
	 *	@brief		Constructor.
	 */
	ObstacleContext();

	/*!
	 *	@brief		Destructor.
	 */
	virtual ~ObstacleContext();

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
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
  virtual MengeVis::SceneGraph::ContextResult handleMouse( QMouseEvent * evt, GLWidget * view );

	/*!
	 *	@brief		Give the context the opportunity to respond to a keyboard
	 *				event.
	 *
	 *	@param		e		The QT event with the keyboard event data.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
  virtual MengeVis::SceneGraph::ContextResult handleKeyboard( QKeyEvent * evt, GLWidget * view );

	/*!
	*	@brief		Returns the control widget associated with this context.
	*
	*				The main application can choose to populate a tool window with this widget.  It contains
	*				buttons and settings to interact with or change state in the context.
	*
	*	@returns	A pointer to the tool widget for this context.
	*/
	virtual QWidget * getContextWidget();

	///////////////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Changes the context mode to drawing a polygon.
	 *
	 *	@returns	True if the context state changed, false if not.
	 */
	bool setPolygonDraw();

	/*!
	 *	@brief		Changes the context mode to editing a polygon.
	 *
	 *	@returns	True if the context state changed, false if not.
	 */
	bool setPolygonEdit();

	/*!
	 *	@brief		Starts a new obstacle set.
	 */
	void startObstacleSet();

	/*!
	 *	@brief		Reports if the context has a live obstacle set already.
	 */
	bool hasLiveObstacleSet();

	/*!
	 *	@brief		Finalizes the currently live obstacle set -- no op if there is
	 *				no live obstacle set.
	 *
	 *	@param		save		If true, the current live obstacle will be saved to the
	 *							scene as an ExplicitObstacleSet.
	 */
	void finishObstacleSet( bool save );

	/*!
	 *	@brief		Sets the obstacle set that the context is working with.
	 *
	 *	@param		obstacleSet		The obstacle set to edit -- if null is passed,
	 *								no obstacle set will be edited.
	 */
	void setObstacleSet(Menge::Agents::ObstacleSet * obstacleSet);

signals:
	
	/*!
	 *	@brief		Emitted when a new obstacle set is created.
	 *
	 *				The new obstacle set has been placed into a static queue in this context.
	 *				It is the responsibility of the recipient to pop the obstacle set off the queue
	 *				This implies that there can only be one recipient.
	 */
	void newObstacleSet();

protected:

	/*!
	*	@brief		Draw context elements into the 3D world.
	*
	*	@param		select		Defines if the drawing is being done for selection
	*							purposes (true) or visualization (false).
	*/
	virtual void draw3DGL(bool select = false);

	/*!
	 *	@Brief		The currently editable obstacle set.  The context does *not* own this obstacle set.
	 */
	LiveObstacleSet * _obstacleSet;

	/*!
	 *	@brief		The set of contexts available to this context to perform context operations.
	 */
	std::vector<QtContext *> _operationContexts;

	/*!
	 *	@brief		Container for the new polygon callback functor.
	 */
	PolygonCreatedCB * _callback;

	/*!
	 *	@brief		Controls the state of the context's operations.
	 */
	ObstacleVerb _state;

	/*!
	 *	@brief		Reports if the polygons being worked with represent a change to a previously-existin gset.
	 */
	bool _editSet;

	/*!
	 *	@brief		The widget for this context.
	 */
	ObstacleContextWidget * _widget;
};

#endif		// __DRAW_OBSTACLE_CONTEXT_H__