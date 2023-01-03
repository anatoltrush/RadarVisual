#ifndef VISUALGL_H
#define VISUALGL_H

#include <QtGui>
#include <QOpenGLWidget>

class VisualGL : public QOpenGLWidget
{
public:
    VisualGL(QWidget *parent = 0);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* pe) override;
    void mouseMoveEvent(QMouseEvent* pe) override;
    void wheelEvent(QWheelEvent *pe) override;

private:
    QVector3D rotation;
    QVector3D scale;

    QPoint mousePos;

    void drawAxises();
};

#endif // VISUALGL_H
