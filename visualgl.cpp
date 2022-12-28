#include "visualgl.h"

VisualGL::VisualGL(QWidget *parent):QOpenGLWidget(parent){

}

void VisualGL::initializeGL(){
    glClearColor(0.5f, 0.55f, 0.6f, 0.5f); // заполняем экран цветом
    glEnable(GL_DEPTH_TEST); // задаем глубину проверки пикселей
    //glEnable(GL_CULL_FACE); // говорим, что будем строить только внешние поверхности
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // фигуры будут закрашены с обеих сторон
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // видны только ребра
}

void VisualGL::resizeGL(int w, int h){

}

void VisualGL::paintGL(){
    drawAxises();
}

void VisualGL::mousePressEvent(QMouseEvent *pe){
    mousePos = pe->pos();
}

void VisualGL::mouseMoveEvent(QMouseEvent *pe){

}

void VisualGL::mouseReleaseEvent(QMouseEvent *pe){

}

void VisualGL::wheelEvent(QWheelEvent *pe){

}

void VisualGL::keyPressEvent(QKeyEvent *event){

}

void VisualGL::drawAxises(){
    float axisLegth = 10.0f;
    glLineWidth(1.0f); // line width

    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(axisLegth, 0, 0);

    glColor3f(1, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, axisLegth, 0);

    glColor3f(0, 1, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, axisLegth);
    glEnd();
}
