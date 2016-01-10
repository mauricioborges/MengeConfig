#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QMatrix4x4>

#include <memory>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

namespace Menge {
	namespace SceneGraph {
		class GLScene;
		class GLCamera;
		class GLLight;
	}
}

class SceneViewer;
class GridNode;

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
	void setScene(Menge::SceneGraph::GLScene * scene);

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
	void setCameraFarPlane(int i, float dist);// { _cameras[i].setFarPlane(dist); }

	friend class SceneViewer;

public slots:
	/*!
	 *	@brief		Cleans up the OpenGL state when the OpenGL context is lost.
	 */
    void cleanup();

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
	 *	@param		state		True to set to perspective, false to set to orthographic.
	 */
	void toggleProjection(bool isPerspective);

public slots:
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
	Menge::SceneGraph::GLScene *	_scene;

	/*!
	 *	@brief		A set of cameras from which to draw the scene.
	 */
	std::vector< Menge::SceneGraph::GLCamera > _cameras;

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
	std::vector< Menge::SceneGraph::GLLight > _lights;

	/*!
	 *	@brief		Determines whether a world-aligned axis is drawn
	 */
	bool	_drawWorldAxis;

	/*!
	 *	@brief		Initizlies the OpenGL lighting based on the set of lights.
	 */
	void initLighting();

	/*!
	 *	@brief		Draws a simple, three-color world axis at the origin of world space.
	 */
	void drawWorldAxis();

	/*!
	 *	@brief		The reference grid for the scene.  The viewer does *not* own this pointer.
	 *				It is added to the scene and managed by the scene.  This is merely a convenience
	 *				reference.
	 */
	GridNode * _grid;

};

#endif
