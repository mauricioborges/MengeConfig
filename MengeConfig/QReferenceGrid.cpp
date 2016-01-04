#include "QReferenceGrid.h"
#include <QtGui/QOpenGLShaderProgram>



/////////////////////////////////////////////////////////////////////////////////////////////
//						OpenGL constants -- TODO: refactor this
/////////////////////////////////////////////////////////////////////////////////////////////

/*!
*	@brief		The GLSL code for the vertex shader.
*/
static const char *vertexShader =
"#version 150\n"
"in vec4 vertex;\n"
"in float alpha;\n"
"out vec3 vert;\n"
"out float alphaOut;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"void main() {\n"
"   alphaOut = alpha;"
"   vert = vertex.xyz;\n"
"   gl_Position = projMatrix * mvMatrix * vertex;\n"
"}\n";

/*!
*	@brief		The GLSL code for the fragment shader.
*/
static const char *fragmentShader =
"#version 150\n"
"in float alpha;\n"
"in highp vec3 vert;\n"
"out highp vec4 fragColor;\n"
"void main() {\n"
"   fragColor = vec4(0.0, 0.0, 0.0, alpha);\n"
"}\n";

QOpenGLShaderProgram * QReferenceGrid::_program = 0x0;
int QReferenceGrid::m_projMatrixLoc = 0;
int QReferenceGrid::m_mvMatrixLoc = 0;
QOpenGLBuffer QReferenceGrid::_vbo;

/////////////////////////////////////////////////////////////////////////////////////////////
//						Implementation of QReferenceGrid
/////////////////////////////////////////////////////////////////////////////////////////////

QReferenceGrid::QReferenceGrid()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

QReferenceGrid::~QReferenceGrid()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////

void QReferenceGrid::initializeGL() {

	_program = new QOpenGLShaderProgram;
	_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
	_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
	_program->bindAttributeLocation("vertex", 0);
	_program->bindAttributeLocation("alpha", 1);
	_program->link();

	_program->bind();
	m_projMatrixLoc = _program->uniformLocation("projMatrix");
	m_mvMatrixLoc = _program->uniformLocation("mvMatrix");

	// Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
	// implementations this is optional and support may not be present
	// at all. Nonetheless the below code works in all cases and makes
	// sure there is a VAO when one is needed.
	//_vao.create();
	//QOpenGLVertexArrayObject::Binder vaoBinder(&_vao);

	// Setup our vertex buffer object.
	_vbo.create();
	_vbo.bind();
	_vbo.allocate(0);// m_logo.constData(), m_logo.count() * sizeof(GLfloat));

	// Store the vertex attribute bindings for the program.
	_vbo.bind();
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	f->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
	_vbo.release();

	_program->release();
}