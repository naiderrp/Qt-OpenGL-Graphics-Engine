#include "oglwidget.h"
#include <dimensional.h>
#include <camera.h>
#include "serializer.h"
#include <skybox.h>
#include <material.h>

#include <QMouseEvent>
#include <QOpenGLContext>
#include <QtMath>
#include <QWheelEvent>

constexpr auto WIDTH = 140.0f;

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_CurrentSkybox(0)
{
    m_Eye = new camera;
    m_Eye->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

OGLWidget::~OGLWidget()
{
    makeCurrent(); // убирает варнинг "QOpenGLTexturePrivate::destroy() called without a current context"

    for(auto obj : m_Objects) delete obj;
    for(auto sbox : skyboxes) delete sbox;

    delete m_Eye;
}

bool OGLWidget::upload(const QString &path)
{
    if (path.isEmpty()) return false;

    for(auto obj : m_Objects) delete obj;
    for(auto sbox : skyboxes) delete sbox;

    m_Objects.clear(); skyboxes.clear();

    serializer::unmarshalize(m_Objects, skyboxes, path);

    update();

    return true;
}

void OGLWidget::save(const QString &path)
{
    serializer::marshalize(m_Objects, skyboxes, path);
}

void OGLWidget::add_object(WorldEngineObject *obj, const QString texture)
{
    dimensional* d = dynamic_cast<dimensional*>(obj);

    if (d != nullptr) load_scene_object(d, texture);
    else add_skybox(dynamic_cast<skybox*>(obj), texture);
}


void OGLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    initCube(10.0f, 10.0f, 10.0f, ":/cube3.png", ":/ice1_n.jpg");
    initCube(10.0f, 10.0f, 10.0f, ":/cube2.png", ":/ice1_n.jpg");
    m_Objects.last()->translate(QVector3D(11.0f, 0.0f, 0.0f));
    initCube(10.0f, 10.0f, 10.0f, ":/cube1.png", ":/ice1_n.jpg");
    m_Objects.last()->translate(QVector3D(11.0f, 0.0f, -12.0f));
    initCube(10.0f, 10.0f, 10.0f, ":/ice1_n.jpg", ":/ice1_n.jpg");
    m_Objects.last()->translate(QVector3D(0.0f, 0.0f, -12.0f));
    /*
    m_Objects.last()->translate(QVector3D(0.0f, 0.0f, -7.0f));
    m_Objects.last()->rotate(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 10.0f));*/
}

void OGLWidget::add_skybox(skybox* s, const QString &texture)
{
    s->init(WIDTH, texture);

    static float pos = skyboxes.isEmpty() ? 0.0f : skyboxes.last()->width();

    skyboxes.append(s);

    skyboxes.last()->translate(QVector3D(pos, 0.0f, 0.0f));

    pos += skyboxes.last()->width();

    update();
}

void OGLWidget::resizeGL(int w, int h)
{
    float aspect = w / (h? static_cast<float>(h) : 1.0f);
    m_PojectionMatrix.setToIdentity();
    m_PojectionMatrix.perspective(45, aspect, 0.01f, 1000.0f); // посл. 2 параметра - настройка плоскостей отсечения
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramSkyBox.bind();
    m_ProgramSkyBox.setUniformValue("u_projectionMatrix", m_PojectionMatrix);
    m_Eye->draw(&m_ProgramSkyBox);

    for (qsizetype i = 0; i < skyboxes.size(); ++i) {
        skyboxes[i]->draw(&m_ProgramSkyBox, context()->functions());
    }

    m_ProgramSkyBox.release();

    m_Program.bind();
    m_Program.setUniformValue("u_projectionMatrix", m_PojectionMatrix);
    m_Program.setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    m_Program.setUniformValue("u_lightPower", 1.0f);
    m_Eye->draw(&m_Program);
    for (qsizetype i = 0; i < m_Objects.size(); ++i)
        m_Objects[i]->draw(&m_Program, context()->functions());
    m_Program.release();
}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {

        m_MousePosition = QVector2D(event->position());

        return;
    }

    int picked = SelectObject(event->position().x(), event->position().y(), m_Objects);

    if (!picked) return;

    --picked;

    auto& materials = m_Objects.at(picked)->GetMaterials();

    if (are_picked.contains(picked)) {

        for(qsizetype i = 0; i < materials.size(); ++i)
              m_Objects.at(picked)->getObject(i)->reset_material();

        picked_items.remove(m_Objects.at(picked));
        are_picked.remove(picked);
    }

    else {

        for(qsizetype i = 0; i < materials.size(); ++i) {
              m_Objects.at(picked)->getObject(i)->GetMaterial()->disableDiffuseMap();
              m_Objects.at(picked)->getObject(i)->GetMaterial()->setDiffuseColor(QVector3D(0.0f, 1.0f, 1.0f));
        }

        picked_items.insert(m_Objects.at(picked));
        are_picked.insert(picked);
    }

    update();
}

void OGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QVector2D diffpos = QVector2D(event->position()) - m_MousePosition;
        m_MousePosition = QVector2D(event->position());

        float angle = diffpos.length() / 2.0f;

        QVector3D axis = QVector3D(diffpos.y(), diffpos.x(), 0.0f);

        if (!picked_items.isEmpty())
              for(auto obj : picked_items)
                obj->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

        else m_Eye->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

        update();
    }

    event->accept();
}

void OGLWidget::wheelEvent(QWheelEvent *event)
{
    if (!picked_items.isEmpty()) {

        float scale = event->angleDelta().y() > 0 ? 1.2f : 0.9f;

        for(auto obj : picked_items) obj->scale(scale);
    }

    else {

        if(event->angleDelta().y() > 0) m_Eye->translate(QVector3D(0.0f, 0.0f, 0.25f));

        else if(event->angleDelta().y() < 0) m_Eye->translate(QVector3D(0.0f, 0.0f, -0.25f));
    }

    update();
}



void OGLWidget::keyPressEvent(QKeyEvent *event)
{
    static std::function<void(const QVector3D&)> move_active_objects =
        [&](const QVector3D& diff) {

            if (!picked_items.isEmpty())
                for(auto d : picked_items)
                    d->translate(-diff);

            else m_Eye->translate(diff);
        };

    switch (event->key())
    {

    case Qt::Key_R:
        m_Eye->reset();
        m_Eye->translate(QVector3D(-WIDTH * m_CurrentSkybox, 0.0f, 0.0f));
        break;

    case Qt::Key_Delete:
        for (auto obj : picked_items) {
            are_picked.remove(get_obj_index(obj));
              picked_items.remove(obj);
            m_Objects.remove(get_obj_index(obj));
            delete obj;
        }
        break;

    case Qt::Key_A:
        move_active_objects(QVector3D(0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_D:
        move_active_objects(QVector3D(-0.5f, 0.0f, 0.0f));
        break;

    case Qt::Key_W:
        move_active_objects(QVector3D(0.0f, 0.0f, 0.5f));
        break;

    case Qt::Key_S:
        move_active_objects(QVector3D(0.0f, 0.0f, -0.5f));
        break;

    case Qt::Key_Q:
        move_active_objects(QVector3D(0.0f, 0.5f, 0.0f)); // OY направлена вниз!
        break;

    case Qt::Key_E:
        move_active_objects(QVector3D(0.0f, -0.5f, 0.0f));
        break;
    }

    update();
}

void OGLWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}

void OGLWidget::initShaders()
{
    if(! m_Program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertshader.vsh"))
        close();
    if(! m_Program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragshader.fsh"))
        close();

    if(! m_Program.link())
        close();

    if(! m_ProgramSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vsh"))
        close();
    if(! m_ProgramSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.fsh"))
        close();

    if(! m_ProgramSkyBox.link())
        close();

    if (!m_SelectProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vselect.vsh"))
        close();

    if (!m_SelectProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fselect.fsh"))
        close();

    if(! m_SelectProgram.link())
        close();
}

void OGLWidget::initCube(float w, float h, float d, const QString &diffuse_map, const QString& normal_map)
{
    Object3dBuilder texture;
    texture.add_ambience_color(QVector3D(1.0f, 1.0f, 1.0f)).
            add_specular_color(QVector3D(1.0f, 1.0f, 1.0f)).
            add_shines(30.0f).
            add_diffuse_map(diffuse_map).
            add_normal_map(normal_map).
            build_vertex_buffer(w / 2.0f, h / 2.0f, d / 2.0f).
            build_index_buffer();

    dimensional* cube = new dimensional;
    cube->addObject(new object3D(texture.getVertexBuff(), texture.getIndexBuff(), texture.getMaterial()));
    m_Objects.append(cube);
}

bool OGLWidget::init_surface(const QString &diffuse_map, const QString& normal_map)
{
    if (diffuse_map.isEmpty() || normal_map.isEmpty()) return false;

    initCube(100.0f, 2.0f, 100.0f, diffuse_map, normal_map);

    m_Objects.last()->translate(QVector3D(0.0f, -40.0f, 0.0f));

    update();

    return true;
}

void OGLWidget::load_scene_object(dimensional* d, const QString &path)
{
    d->loadObjectFromFile(path);

    d->translate(QVector3D(WIDTH * m_CurrentSkybox, 0.0f, 0.0f));

    m_Objects.append(d);
}

void OGLWidget::switch_skybox(int index)
{
    if (skyboxes.isEmpty()) return;

    m_CurrentSkybox = index;

    m_Eye->reset();
    m_Eye->translate(QVector3D(-WIDTH * m_CurrentSkybox, 0.0f, 0.0f));

    update();
}

int OGLWidget::SelectObject(int x, int y, QVector<dimensional *> &objs)
{
    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); //для корректной работы оси z: дальние объекты не должны перекрывать ближние

    m_SelectProgram.bind();
    m_SelectProgram.setUniformValue("u_projectionMatrix", m_PojectionMatrix);
    m_Eye->draw(&m_SelectProgram);

    for (qsizetype i = 0; i < objs.size(); ++i) {

        m_SelectProgram.setUniformValue("u_code", float(i + 1));
        objs[i]->draw(&m_SelectProgram, context()->functions(), false);
    }

    m_SelectProgram.release();

    GLint viewport[4]; //x, y, w, h
    glGetIntegerv(GL_VIEWPORT, viewport);

    unsigned char res[4]; //4 компоненты RGBA, каждый по байту, [0-255], поэтому такой тип данных

    //1, 1 - ширина и высота пикселя который нужно считать под указателем мыши
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

    glDisable(GL_DEPTH_TEST);

    return res[0]; //красная компонента
}

int OGLWidget::get_obj_index(dimensional *obj)
{
    for(int i = 0; i < m_Objects.size(); ++i)
        if (m_Objects[i] == obj) return i;

    return -1;
}
