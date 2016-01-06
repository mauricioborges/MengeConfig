#ifndef _Q_REFERENCE_GRID_H__
#define _Q_REFERENCE_GRID_H__

#include "ReferenceGrid.h"

#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QMatrix4x4>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

/*!
 *	@brief		Handles the visualization of the reference grid in a QT context.
 */
class QReferenceGrid : public ReferenceGrid
{
public:
	/*!
	 *	@brief		Constructor.
	 */
	QReferenceGrid();

	/*!
	 *	@brief		Destructor.
	 */
	~QReferenceGrid();

private:
	static QOpenGLShaderProgram * _program;

	static int m_projMatrixLoc;
	static int m_mvMatrixLoc;
	QOpenGLVertexArrayObject _vao;
	static QOpenGLBuffer _vbo;

	/*!
	 *	@brief		Initializes the open gl objects.
	 */
	static void initializeGL();
};

#endif	// _Q_REFERENCE_GRID_H__