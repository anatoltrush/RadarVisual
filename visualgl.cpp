#include "visualgl.h"

VisualGL::VisualGL(QWidget *parent):QOpenGLWidget(parent){
    scale = QVector3D(0.1f, 0.1f, 0.1f);
}

void VisualGL::initializeGL(){
    glClearColor(0.5f, 0.55f, 0.6f, 0.5f); // заполняем экран цветом
    glEnable(GL_DEPTH_TEST); // задаем глубину проверки пикселей
    //glEnable(GL_CULL_FACE); // говорим, что будем строить только внешние поверхности
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // фигуры будут закрашены с обеих сторон
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // видны только ребра
}

void VisualGL::resizeGL(int w, int h){
    /*koeff.setX(prevSize.width() / (float)w);
    koeff.setY(prevSize.height() / (float)h);
    koeff.setZ(1.0f);

    scale *= koeff;
    constScale = scale;*/
}

void VisualGL::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1000.0f, 1000.0f, -1000.0f, 1000.0f, -1000.0f, 1000.0f);

    glRotatef(rotation.x(), 1.0, 0.0, 0.0);
    glRotatef(rotation.y(), 0.0, 1.0, 0.0);
    glRotatef(rotation.z(), 0.0, 0.0, 1.0);

    glScalef(scale.x(), scale.y(), scale.z());

    drawAxises();
    // for all in vec
}

void VisualGL::mousePressEvent(QMouseEvent *pe){
    mousePos = pe->pos();
}

void VisualGL::mouseMoveEvent(QMouseEvent *pe){
#if QT_VERSION_MAJOR > 5
    rotation.setX(rotation.x() - (1/scale.y() * (GLfloat)(pe->position().y() - mousePos.y())/height()));
    rotation.setZ(rotation.z() - (1/scale.x() * (GLfloat)(pe->position().x() - mousePos.x())/width()));
#else
    rotation.setX(rotation.x() - (1/scale.z() * (GLfloat)(pe->pos().y() - mousePos.y())/height()));
    rotation.setZ(rotation.z() - (1/scale.z() * (GLfloat)(pe->pos().x() - mousePos.x())/width()));
#endif
    mousePos = pe->pos();

    //float diffXY = constScale.y() - constScale.x();
    //float addScale = diffXY * abs(sin(rotation.degToRad().z()));

    //scale.x = constScale.x + addScale;
    //scale.y = constScale.y - addScale;
    //std::cout << rotation.z << " | " << /*addScale*/ sin(rotation.degToRad().z) << std::endl;
    //std::cout << "scale.x | " << scale.x << std::endl;
    //std::cout << "scale.y | " << scale.y << std::endl;
    update();
}

void VisualGL::wheelEvent(QWheelEvent *pe){
    float wheelScale = 1.1f;
    if ((pe->angleDelta().y()) > 0)
        scale *= wheelScale;
    else scale /= wheelScale;

    update();
}

void VisualGL::drawAxises(){
    float axisLegth = 10.0f;
    glLineWidth(1.0f); // line width

    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(-axisLegth, 0, 0);
    glVertex3f(axisLegth, 0, 0);

    glColor3f(1, 1, 0);
    glVertex3f(0, -axisLegth, 0);
    glVertex3f(0, axisLegth, 0);

    glColor3f(0, 1, 1);
    glVertex3f(0, 0, -axisLegth);
    glVertex3f(0, 0, axisLegth);
    glEnd();
}
