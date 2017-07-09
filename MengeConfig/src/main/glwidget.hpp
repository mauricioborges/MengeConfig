#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QMatrix4x4>

#include <MengeCore/Math/Vector2.h>

#include <memory>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

namespace MengeVis {
	namespace SceneGraph {
		class GLScene;
		class GLCamera;
		class GLLight;
	}
}

class SceneViewer;
class GridNode;
class QtContext;

/*!
 *	@brief		The view that contains the open gl context.
 */
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
	/*!
	 *	@biref		Constructor.
	 *
	 *	@param		parent		The optional parent widget.
	 */
    GLWidget(QWidget *parent = 0);

	/*!
	 *	@brief		Destructor.
	 */
    ~GLWidget();

	/*!
	 *	@brief		Override defining the minimum size for the window.
	 *
	 *	@returns	The minimum size for this view.
	 */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

	/*!
	 *	@brief		The hint for the size of this widget.
	 *
	 *	@returns	The hinted size for this widget.
	 */
	QSize sizeHint() const Q_DECL_OVERRIDE;

	/*!
	 *	@brief		Sets the GLScene to be drawn.
	 *				A viewer can only draw a single scene; all drawn
	 *				elements belong either to the GLScene or a GLContext.
	 *
	 *	@param		scene	The GLScene to be rendered and temporally advanced.
	 */
	void setScene(MengeVis::SceneGraph::GLScene * scene);

	/*!
	 *	@brief		Sets the horizontal field of view for the ith camera.
	 *
	 *	@param		i		The index of the camera - index value is NOT checked.
	 *	@param		fov		The horizontal field of view for the indicated camera.
	 *						Value is not validated.
	 */
	void setCameraFOV(int i, float fov);

	/*!
	 *	@brief		Sets the distance to the camera's far clipping plane.
	 *
	 *	@param		i		The index of the camera - index value is NOT checked.
	 *	@param		dist	The distance, from the camera, to its far clipping plane.
	 *						Value is not validated.
	 */
	void setCameraFarPlane(int i, float dist);

	friend class SceneViewer;

public slots:
	/*!
	 *	@brief		Cleans up the OpenGL state when the OpenGL context is lost.
	 */
  void cleanup();

	/*!
	 *	@brief		Reports to the viewer that a new context has been activated.
	 *
	 *	@param		id		The identifier for the context.
	 */
	void activated(size_t id);

	/*!
	 *	@brief		Reports to the viewer that a context has been deactivated.
	 *
	 *	@param		id		The identifier for the context.
	 */
	void deactivated(size_t id);

signals:
	/*!
	 *	@brief		Indicates that the camera has been rotated arbitrarily.
	 */
	void userRotated();

	/*!
	 *	@brief		Reports the "current" world position of the point under the mouse.
	 *
	 *	The world position lies on the simulation's x-y plane. 
	 *	
	 *	@param		x		The x-value of the position.
	 *	@param		y		The y-value of the position.
	 */
	void currWorldPos(float x, float y);

protected:
	/*!
	 *	@brief		Initializes the OpenGL state
	 */
  void initializeGL() Q_DECL_OVERRIDE;

	/*!
	 *	@brief		Draws the scene to the OpenGL window.
	 */
  void paintGL() Q_DECL_OVERRIDE;

	/*!
	 *	@brief		Provides the new size of the GL window.
	 *
	 *	@param		width		The width of the window (in pixels).
	 *	@param		height		The height of the window (in pixels).
	 */
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;

	/*!
	*	@brief		The callback for when a mouse is pressed.
	*
	*	@param		event		The event parameters.
	*/
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

	/*!
	*	@brief		The callback for when a mouse is pressed.
	*
	*	@param		event		The event parameters.
	*/
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

	/*!
	 *	@brief		The callback for when the mouse is moved.
	 *
	 *	@param		event		The event parameters.
	 */
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

	/*!
	*	@brief		The callback for when mouse wheel is turned.
	*
	*	@param		event		The event parameters.
	*/
	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

	/*!
	*	@brief		The callback for when a key is pressed.
	*
	*	@param		event		The event parameters.
	*/
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

	/*!
	*	@brief		The callback for when a key is released.
	*
	*	@param		event		The event parameters.
	*/
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

	/*!
	 *	@brief		Causes the viewer to recognize that a new OpenGL context has been
	 *				created (such as window resizes).
	 */
	void newGLContext();

	/*!
	 *	@brief		Sets the views state for drawing the axis.
	 *
	 *	@param		state		True to draw it, false to hide it.
	 */
	void setDrawAxis(bool state);

	/*!
	 *	@brief		Toggles current camera between perspective and orthographic projection.
	 *
	 *	@param		isPerspective		True to set to perspective, false to set to orthographic.
	 */
	void toggleProjection(bool isPerspective);

	/*!
	 *	@brief		Toggles whether or not the grid is drawn in the scene.
	 *				A grid that is not visible cannot be used for snapping.
	 *
	 *	@param		isActive		True to enable grid drawing, false to disable.
	 */
	void toggleReferenceGrid(bool isActive);

	/*!
	 *	@brief		Toggles whether selection points are snapped to the horizontal
	 *				lines of the *active* grid.
	 *
	 *	@param		isActive		True enables snapping, false disabled.
	 */
	void toggleHorizontalSnap(bool isActive);

	/*!
	 *	@brief		Toggles whether selection points are snapped to the vertical
	 *				lines of the *active* grid.
	 *
	 *	@param		isActive		True enables snapping, false disabled.
	 */
	void toggleVerticalSnap(bool isActive);

public:

	/*!
	 *	@brief		Returns the position under the mouse on the ground plane in
	 *				world space.  
	 *
	 *	@param		screenPos		The position of the mouse in screen space.
	 *	@param		worldPos		The point on the world x-y plane under the mouse.
	 *	@param		ignoreSnap		If set to true, the world position will ignore the
	 *								snap-to-grid settings.
	 *	@returns	True if worldPos has been set (i.e., the value can be computed, false
	 *				otherwise.)
	 */
	bool getWorldPos(const QPoint & screenPos, Menge::Math::Vector2 & worldPos, bool ignoreSnap = false);

	/*!
	 *	@brief		Snaps the given world position to the grid (according to settings).
	 *				
	 *	If the settings have all snap settings turned off, the returned values will be
	 *	equal to the input.
	 *
	 *	@param		pos		The position to snap (conditionally).
	 *	@returns	The snapped position.
	 */
	Menge::Math::Vector2 snap(const Menge::Math::Vector2 & pos);

	/*!
	 *	@brief		Given a length in screen space, returns the world space
	 *				length based on current view parameters on the ground plane.
	 *
	 *	@param		len		The length to scale.
	 *	@returns	The len scaled to world space.
	 */
	float getWorldScale(float len);

public slots:

	/*!
	 *	@brief		Open the dialog to edit the reference grid's properties.
	 */
	void editGridProperties();

	/*!
	 *	@brief		Re-orients the camera to look down a specific axis.
	 *
	 *	@param		direction	An encoded direction: 0-5: [x, -x, y, -y, z, -z]
	 */
	void setViewDirection(int direction);

protected:
	/*!
	 *	@brief		The GLScene to draw.
	 */
	MengeVis::SceneGraph::GLScene *	_scene;

	/*!
	 *	@brief		The active context on the scene.  The view does *not* own the context.
	 */
	QtContext * _context;

	/*!
	 *	@brief		A set of cameras from which to draw the scene.
	 */
	std::vector< MengeVis::SceneGraph::GLCamera > _cameras;

	/*!
	 *	@brief		The index of camera currently being used to draw the scene.
	 */
	size_t		_currCam;

	/*!
	 *	@brief		The position of the mouse when the button was depressed (in screen space).
	 */
	QPoint _downPos;

	/*!
	 *	@brief		The set of lights to use in rendering.
	 */
	std::vector< MengeVis::SceneGraph::GLLight > _lights;

	/*!
	 *	@brief		Determines whether a world-aligned axis is drawn
	 */
	bool	_drawWorldAxis;

	/*!
	 *	@brief		Determines if the reference grid is active.
	 */
	bool	_activeGrid;

	/*!
	 *	@brief		Reports if the selection point should be snapped to the
	 *				*active* grid's horizontal lines.
	 */
	bool	_hSnap;

	/*!
	 *	@brief		Reports if the selection point should be snapped to the
	 *				*active* grid's vertical lines.
	 */
	bool	_vSnap;

	/*!
	 *	@brief		Reports if the camera is in a top-view configuration.
	 */
	bool	_isTopView;

	/*!
	 *	@brief		The reference grid for the scene.  The viewer does *not* own this pointer.
	 *				It is added to the scene and managed by the scene.  This is merely a convenience
	 *				reference.
	 */
	GridNode * _grid;

	/*!
	 *	@brief		Initizlies the OpenGL lighting based on the set of lights.
	 */
	void initLighting();

	/*!
	 *	@brief		Draws a simple, three-color world axis at the origin of world space.
	 */
	void drawWorldAxis();

  //////////////////////////////////////////////////////////
  //         Menge artifacts

};

#endif
