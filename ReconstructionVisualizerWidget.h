#pragma once

#include <QMouseEvent>

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

class ReconstructionVisualizerWidget : public QOpenGLWidget
{
public:
	explicit ReconstructionVisualizerWidget(QWidget* parent = nullptr);

public slots:
	void addPoint(const QVector3D& vec, const QColor& color = Qt::black);
	void addPoints(const QVector<QVector3D>& points);
	void refreshPointBuffer();

protected:
	void mousePressEvent(QMouseEvent* mouseEvent);
	void mouseMoveEvent(QMouseEvent* mouseEvent);
	void mouseReleaseEvent(QMouseEvent* mouseEvent);
	
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

private:
	void buildPointsVao();
	void buildAxisVao();

	float _yRotation;

	GLuint _pointsVertexArrayObject;
	GLuint _pointsVertexBufferId;
	GLuint _pointsColorBufferId;

	QOpenGLShaderProgram* _shaderProgram;

	std::vector<GLfloat> _pointsGeometry;
	std::vector<GLfloat> _pointsColors;

	GLuint _axisVertexArrayObject;
	GLuint _axisVertexBufferId;
	GLuint _axisColorBufferId;

	std::vector<GLfloat> _axisGeometry;
	std::vector<GLfloat> _axisColors;
	std::vector<GLint> _axisElements;

	QPoint _lastMousePosition;
};

