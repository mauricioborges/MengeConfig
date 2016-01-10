/*!
 *	@file		GridNode.h
 *				The visualization of a reference grid.
 */

#ifndef __GRID_NODE_H__
#define __GRID_NODE_H__

#include <GLNode.h>
#include "ReferenceGrid.h"

class GridNode : public Menge::SceneGraph::GLNode, public ReferenceGrid {
public:
	/*!
	 *	@brief		Constructor.
	 *
	 *	@param		parent		A pointer to the optional parent node in the
	 *							graph.
	 */
	GridNode(Menge::SceneGraph::GLDagNode * parent = 0x0);

	/*!
	 *	@brief		Virtual destructor.
	 */
	virtual ~GridNode();

	/*!
	 *	@brief		Causes this node to draw itself to the scene.
	 *
	 *	This is a purely virtual function.  Every node in the
	 *	graph *must* implement this function.
	 *
	 *	@param		select		Determines if the draw call is being performed
	 *							for the purpose of selection (true) or for visualization
	 *							(false).
	 */
	virtual void drawGL(bool select = false);

	/*!
	 *	@brief		Allows the node to recreate any unique OpenGL objects
	 *				based on the acquisition of a new OpenGL context.
	 *
	 *	This should be over-ridden by nodes that have their own
	 *	unique OpenGL constructs (display lists, textures, etc.)
	 */
	virtual void newContext();

protected:

};

#endif	// __GRID_NODE_H__