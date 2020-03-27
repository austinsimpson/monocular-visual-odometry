#include "ReconstructionVisualizerWidget.h"

#include "Utilities.h"

#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>

ReconstructionVisualizerWidget::ReconstructionVisualizerWidget
(
	QWidget* parent
): 
	QOpenGLWidget(parent),
	_lastMousePosition(-1, -1),
	_pointsColorBufferId(-1),
	_pointsVertexArrayObject(-1),
	_pointsVertexBufferId(-1),
	_shaderProgram(nullptr),
	_yRotation(180.0f)
{
	
}

void ReconstructionVisualizerWidget::initializeGL()
{
	QOpenGLFunctions* glFunctions = QOpenGLContext::currentContext()->functions();
	QColor cornflowerBlue("cornflower blue");
	glFunctions->glClearColor((double)cornflowerBlue.red() / 255, (double)cornflowerBlue.green() / 255, (double)cornflowerBlue.blue() / 255, 1.0);
	
	_shaderProgram = new QOpenGLShaderProgram(this);
	_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.shader");
	_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "fragment.shader");

	bool linked = _shaderProgram->link();
	_shaderProgram->bind();
	
	buildPointsVao();
	buildAxisVao();

	glFunctions->glBindBuffer(GL_ARRAY_BUFFER, _pointsVertexBufferId);
	glFunctions->glEnable(GL_PROGRAM_POINT_SIZE);
	glFunctions->glEnable(GL_POINT_SMOOTH);

	addPoint(QVector3D(1.0, 1.0, 1.0), QColor("pink"));
	refreshPointBuffer();
}

void ReconstructionVisualizerWidget::buildPointsVao()
{
	_pointsGeometry = {
		0.0f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	_pointsColors = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};

	auto glFunctions = QOpenGLContext::currentContext()->functions();
	auto glExtraFunctions = QOpenGLContext::currentContext()->extraFunctions();
	glExtraFunctions->glGenVertexArrays(1, &_pointsVertexArrayObject);
	glExtraFunctions->glBindVertexArray(_pointsVertexArrayObject);

	glFunctions->glGenBuffers(1, &_pointsVertexBufferId);
	glFunctions->glBindBuffer(GL_ARRAY_BUFFER, _pointsVertexBufferId);
	glFunctions->glBufferData(GL_ARRAY_BUFFER, _pointsGeometry.size() * sizeof(GLfloat), _pointsGeometry.data(), GL_STATIC_DRAW);

	GLint positionAttribute = _shaderProgram->attributeLocation("position");
	glFunctions->glEnableVertexAttribArray(positionAttribute);
	glFunctions->glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glFunctions->glGenBuffers(1, &_pointsColorBufferId);
	glFunctions->glBindBuffer(GL_ARRAY_BUFFER, _pointsColorBufferId);
	glFunctions->glBufferData(GL_ARRAY_BUFFER, _pointsColors.size() * sizeof(GLfloat), _pointsColors.data(), GL_STATIC_DRAW);

	GLint colorAttribute = _shaderProgram->attributeLocation("inColor");
	glFunctions->glEnableVertexAttribArray(colorAttribute);
	glFunctions->glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void ReconstructionVisualizerWidget::buildAxisVao()
{
	_axisGeometry = {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
	};

	_axisColors = {
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	};

	_axisElements = {
		0, 1,
		0, 2,
		0, 3
	};

	auto glFunctions = QOpenGLContext::currentContext()->functions();
	auto glExtraFunctions = QOpenGLContext::currentContext()->extraFunctions();

	glExtraFunctions->glGenVertexArrays(1, &_axisVertexArrayObject);
	glExtraFunctions->glBindVertexArray(_axisVertexArrayObject);

	glFunctions->glGenBuffers(1, &_axisVertexBufferId);
	glFunctions->glBindBuffer(GL_ARRAY_BUFFER, _axisVertexBufferId);
	glFunctions->glBufferData(GL_ARRAY_BUFFER, _axisGeometry.size() * sizeof(GLfloat), _axisGeometry.data(), GL_STATIC_DRAW);
	GLint positionAttribute = _shaderProgram->attributeLocation("position");
	glFunctions->glEnableVertexAttribArray(positionAttribute);
	glFunctions->glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glFunctions->glGenBuffers(1, &_axisColorBufferId);
	glFunctions->glBindBuffer(GL_ARRAY_BUFFER, _axisColorBufferId);
	glFunctions->glBufferData(GL_ARRAY_BUFFER, _axisColors.size() * sizeof(GLfloat), _axisColors.data(), GL_STATIC_DRAW);
	GLint colorAttribute = _shaderProgram->attributeLocation("inColor");
	glFunctions->glEnableVertexAttribArray(colorAttribute);
	glFunctions->glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void ReconstructionVisualizerWidget::addPoint
(
	const QVector3D& vec,
	const QColor& color
)
{
	_pointsGeometry.push_back(vec.x());
	_pointsGeometry.push_back(vec.y());
	_pointsGeometry.push_back(vec.z());

	_pointsColors.push_back(color.redF());
	_pointsColors.push_back(color.greenF());
	_pointsColors.push_back(color.blueF());
}

void ReconstructionVisualizerWidget::addPoints
(
	const QVector<QVector3D>& points
)
{
	_pointsGeometry.clear();
	_pointsColors.clear();
	for (auto point : points)
	{
		_pointsGeometry.push_back(point.x());
		_pointsGeometry.push_back(point.y());
		_pointsGeometry.push_back(point.z());

		QColor pointColor = colorForPoint((int)point.x(), (int)point.y(), 640, 480);
		_pointsColors.push_back((double)rand() / RAND_MAX);
		_pointsColors.push_back((double)rand() / RAND_MAX);
		_pointsColors.push_back((double)rand() / RAND_MAX);
	}
	refreshPointBuffer();
}

void ReconstructionVisualizerWidget::refreshPointBuffer()
{
	auto glFunctions = QOpenGLContext::currentContext()->functions();
	auto glExtraFunctions = QOpenGLContext::currentContext()->extraFunctions();

	glExtraFunctions->glBindVertexArray(_pointsVertexArrayObject);
	glFunctions->glBindBuffer(GL_ARRAY_BUFFER, _pointsVertexBufferId);
	glFunctions->glBufferData(GL_ARRAY_BUFFER, _pointsGeometry.size() * sizeof(GLfloat), _pointsGeometry.data(), GL_STATIC_DRAW);

	glFunctions->glBindBuffer(GL_ARRAY_BUFFER, _pointsColorBufferId);
	glFunctions->glBufferData(GL_ARRAY_BUFFER, _pointsColors.size() * sizeof(GLfloat), _pointsColors.data(), GL_STATIC_DRAW);
}


void ReconstructionVisualizerWidget::resizeGL
(
	int width, int height
)
{
	QOpenGLFunctions* glFunctions = QOpenGLContext::currentContext()->functions();
	QOpenGLExtraFunctions* extraGlFunctions = QOpenGLContext::currentContext()->extraFunctions();
	glFunctions->glViewport(0, 0, width, height);
}

void ReconstructionVisualizerWidget::paintGL()
{
	auto glFunctions = QOpenGLContext::currentContext()->functions();
	auto glExtraFunctions = QOpenGLContext::currentContext()->extraFunctions();

	static double yRot = 0.0;
	QMatrix4x4 transformationMatrix;
	
	transformationMatrix.perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	transformationMatrix.translate(0.0f, 0.0f, -50.0f);
	transformationMatrix.rotate(_yRotation, 0.0f, 1.0f, 0.0f);

	_shaderProgram->setUniformValue("transformation", transformationMatrix);

	const auto retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT);

	glExtraFunctions->glBindVertexArray(_pointsVertexArrayObject);
	glDrawArrays(GL_POINTS, 0, _pointsGeometry.size() / 3);

	glExtraFunctions->glBindVertexArray(_axisVertexArrayObject);
	glDrawArrays(GL_LINES, 0, _axisGeometry.size() / 3);
	yRot += 0.1;

	update();
}

void ReconstructionVisualizerWidget::mousePressEvent
(
	QMouseEvent* mouseEvent
)
{
	_lastMousePosition = mouseEvent->pos();
}

void ReconstructionVisualizerWidget::mouseMoveEvent
(
	QMouseEvent* mouseEvent
)
{
	
	if (_lastMousePosition.x() >= 0 && _lastMousePosition.y() >= 0)
	{
		int xDelta = (mouseEvent->pos() - _lastMousePosition).x();
		_yRotation += xDelta;
	}

	_lastMousePosition = mouseEvent->pos();
}

void ReconstructionVisualizerWidget::mouseReleaseEvent
(
	QMouseEvent* mouseEvent
)
{
	Q_UNUSED(mouseEvent);
	_lastMousePosition.rx() = 0;
	_lastMousePosition.ry() = 0;
}