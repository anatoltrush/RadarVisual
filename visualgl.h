#ifndef VISUALGL_H
#define VISUALGL_H

#include <QtGui>
#include <QOpenGLWidget>

class VisualGL : public QOpenGLWidget
{
public:
    VisualGL(QWidget *parent = 0);

protected:
    void initializeGL() override; // reimplementation
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* pe) override;   // нажатие на клавишу мыши
    void mouseMoveEvent(QMouseEvent* pe) override;    // перемещение мыши
    void mouseReleaseEvent(QMouseEvent* pe) override; // отжатие клавиши мыши
    void wheelEvent(QWheelEvent *pe) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    QVector3D rotation;
    QVector3D scale;

    QPoint mousePos;

    void drawAxises();
};

#endif // VISUALGL_H
