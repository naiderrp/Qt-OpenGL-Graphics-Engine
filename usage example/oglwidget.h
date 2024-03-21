#ifndef OGLWIDGET_H
#define OGLWIDGET_H
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QBasicTimer>

class dimensional;
class WorldEngineObject;
class group;
class camera;
class skybox;

class OGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OGLWidget(QWidget *parent = nullptr);
    ~OGLWidget();

    bool upload(const QString& path);

    void save(const QString& path);

    void add_object(WorldEngineObject* obj, const QString texture);

    bool init_surface(const QString &diffuse_map, const QString& normal_map);

    void switch_skybox(int index);

    void keyPressEvent(QKeyEvent* event);

    QVector<dimensional*> m_Objects;
    QVector<skybox*> skyboxes;
    QSet<dimensional*> picked_items;
    camera* m_Eye;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void timerEvent(QTimerEvent* event);

    void initShaders();
    void initCube(float w, float h, float d, const QString &diffuse_map, const QString& normal_map);

    int SelectObject(int x, int y, QVector<dimensional *> &objs);

    int get_obj_index(dimensional* obj);

    void add_skybox(skybox* s, const QString &texture);

    void load_scene_object(dimensional* d, const QString& path);

private:
    QMatrix4x4 m_PojectionMatrix;

    QOpenGLShaderProgram m_Program;
    QOpenGLShaderProgram m_ProgramSkyBox;
    QOpenGLShaderProgram m_SelectProgram;

    QVector2D m_MousePosition;
    QQuaternion m_Rotation;

    int m_CurrentSkybox;

    QSet<int> are_picked;
};

#endif // OGLWIDGET_H
