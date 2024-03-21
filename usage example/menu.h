#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidgetItem>

#include <graphics_engine.h>
#include "graphics_pipeline.h"
#include <dimensional.h>
#include <skybox.h>


enum DimensionalObjects {
    background,
    scene_object
};

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);
    ~menu();
    void SetBackgroundColor(const QColor& color);
    void SetBackgroundPicture(const QString &path);

    void SetButtonColor(QPushButton* button, const QColor& color);
    void SetButtonTextColor(QPushButton* button, const QColor& color);

    void SetQLabelTextColor(QLabel* label, const QColor& color);
    void SetQLabelTextBorder(QLabel* label, const QColor& color);


protected:
    void AdjustDialogWindow(const QString& question, auto& on_yes_clicked, auto& on_no_clicked, const QString& background);

    void SetDialogWindowInterface();

    void SetButtonsInterface();

    void SetQLabelsInterface();

    void UpdateListWidget();

    void RegisterFactories();

    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_upload_file_pb_clicked();

    void on_init_skybox_pb_clicked();

    void on_exit_pb_clicked();

    void on_fullscreen_pb_clicked();

    void on_save_pb_clicked();

    void on_upload_world_pb_clicked();

    void on_scene_lw_itemClicked(QListWidgetItem *item);

private:
    QWidget w;

    QPushButton* yes_pb;
    QPushButton* no_pb;

    QLabel* question_l;

    Ui::menu *ui;

    QPoint canvas_pos;
    QSize canvas_size;

    GraphicsPipeline<WorldEngineObject, DimensionalObjects> factory;
};

#endif // MENU_H
