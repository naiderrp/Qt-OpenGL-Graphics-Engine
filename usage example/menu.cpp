#include "menu.h"
#include "qevent.h"
#include "ui_menu.h"

#include <QGraphicsDropShadowEffect>

#include <QDesktopServices>
#include <QUrl>

#include <QFileDialog>

#include <QRandomGenerator>


menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);

    SetBackgroundColor(QColor(45, 45, 45));

    SetButtonsInterface();

    SetQLabelsInterface();

    SetDialogWindowInterface();

    RegisterFactories();

    canvas_size = ui->canvas_widget->size();
    canvas_pos = ui->canvas_widget->pos();

    setWindowFlags(Qt::FramelessWindowHint);
}

menu::~menu()
{
    delete ui;
}

void menu::on_upload_file_pb_clicked()
{
    w.show();

    std::function<void()> yes_func = [](){ QDesktopServices::openUrl(QUrl("https://drive.google.com/drive/folders/16Mkt4cJ8LBlcczT-D0DHbvqqXR84Rjcm")); };
    std::function<void()> no_func = [&](){ auto path = QFileDialog::getOpenFileName(&w, "Choose file", "C:/Users/Admin", "obj (*.obj);");
                                           if (path.isEmpty()) return;
                                           ui->canvas_widget->add_object(factory.pullout(DimensionalObjects::scene_object), path);
                                           w.hide(); };

    AdjustDialogWindow("would you like to\ndownload a 3D model first?", yes_func, no_func, ":/hearts2.jpg");
}


void menu::on_init_skybox_pb_clicked()
{
    w.show();

    std::function<void()> yes_func = [](){ QDesktopServices::openUrl(QUrl("https://drive.google.com/drive/folders/16Naaaomoq15Q1Hu7VtLTfAhIoNLPp6rf")); };
    std::function<void()> no_func = [&](){ auto path = QFileDialog::getOpenFileName(&w, "Choose file", "C:/Users/Admin", "jpeg (*.jpg);; png (*.png);");
                                           if (path.isEmpty()) return;
                                           ui->canvas_widget->add_object(factory.pullout(DimensionalObjects::background), path);
                                           UpdateListWidget(); w.hide(); };

    AdjustDialogWindow("would you like to\ndownload a texture first?", yes_func, no_func, ":/hearts3.jpg");
}

void menu::on_exit_pb_clicked()
{
    w.show();

    std::function<void()> yes_func = [&](){ ui->canvas_widget->save(QString("saved%1.json").arg(QRandomGenerator::global()->bounded(1, 4000))); w.close(); close(); };
    std::function<void()> no_func = [&](){ w.close(); close();};

    AdjustDialogWindow("do you want to save\nthe progress?", yes_func, no_func, ":/hearts6.jpg");
}


void menu::on_fullscreen_pb_clicked()
{
    showFullScreen();
    ui->canvas_widget->resize(width(), height());
    ui->canvas_widget->move(pos().x() - 8, pos().y());
}


void menu::on_save_pb_clicked()
{
    ui->canvas_widget->save(QString("saved%1.json").arg(QRandomGenerator::global()->bounded(1, 4000)));
}


void menu::on_upload_world_pb_clicked()
{
    w.show();

    std::function<void()> yes_func = [](){ QDesktopServices::openUrl(QUrl("https://drive.google.com/drive/folders/16NxB2enwiNliyMbD3RzR51M-ffD4Bk2c")); };
    std::function<void()> no_func = [&](){ auto path = QFileDialog::getOpenFileName(&w, "Choose file", "C:/Users/Admin", "json (*.json);");
                                           if (ui->canvas_widget->upload(path))
                                           { UpdateListWidget(); w.hide(); }};

    AdjustDialogWindow("would you like to\ndownload the json file first?", yes_func, no_func, ":/hearts5.jpg");
}

void menu::SetBackgroundColor(const QColor& color)
{
    setAutoFillBackground(true);

    QPalette pal;
    pal.setColor(QPalette::Window, color);
    setPalette(pal);
    w.setPalette(pal);

    pal.setColor(QPalette::Base, Qt::transparent);
    ui->scene_lw->setPalette(pal);
}


void menu::SetBackgroundPicture(const QString &path)
{
    w.setAutoFillBackground(true);

    QPalette pal;

    QPixmap pic(path);

    QBrush br;
    br.setTexture(pic.scaled(w.width(), w.height()));

    pal.setBrush(QPalette::Window, br);

    w.setPalette(pal);
}

void menu::SetButtonColor(QPushButton *button, const QColor& color)
{
    button->setStyleSheet(QString("background: %1").arg(color.name()));
}

void menu::SetButtonTextColor(QPushButton *button, const QColor &color)
{
    QPalette pal;
    pal.setColor(QPalette::ButtonText, color);

    button->setPalette(pal);
}

void menu::SetQLabelTextColor(QLabel* label, const QColor& color)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, color);

    label->setPalette(pal);
}

void menu::SetQLabelTextBorder(QLabel *label, const QColor &color)
{
    static auto effect = new QGraphicsDropShadowEffect(&w);
    effect->setOffset(-1, -1);
    effect->setColor(color);
    label->setGraphicsEffect(effect);
}

void menu::AdjustDialogWindow(const QString &question, auto& on_yes_clicked, auto& on_no_clicked, const QString& background)
{
    SetBackgroundPicture(background);

    question_l->setText(question);

    static QMetaObject::Connection yes_connection;
    static QMetaObject::Connection no_connection;

    w.disconnect(yes_connection);  w.disconnect(no_connection);

    yes_connection = connect(yes_pb, &QPushButton::clicked, on_yes_clicked);
    no_connection = connect(no_pb, &QPushButton::clicked, on_no_clicked);
}

void menu::SetDialogWindowInterface()
{
    w.setGeometry(400, 200, 700, 400);

    question_l = new QLabel(&w);

    QFont font("Sitka");
    font.setWeight(QFont::Bold);
    font.setPixelSize(45);

    question_l->setFont(font);

    question_l->setGeometry(0, 0, 515, 356);
    question_l->move(130, -30);

    yes_pb = new QPushButton(&w);
    no_pb = new QPushButton(&w);

    yes_pb->setFont(font); no_pb->setFont(font);
    yes_pb->setText("yes"); no_pb->setText("no");

    yes_pb->setFixedSize(200, 70); no_pb->setFixedSize(200, 70);
    yes_pb->move(100, 250); no_pb->move(400, 250);


    SetButtonColor(yes_pb, Qt::white);
    SetButtonColor(no_pb, Qt::white);
    SetQLabelTextColor(question_l, Qt::white);
    SetQLabelTextBorder(question_l, Qt::black);
}

void menu::SetButtonsInterface()
{
    SetButtonColor(ui->fullscreen_pb, QColor(30, 30, 30)); SetButtonTextColor(ui->fullscreen_pb, QColor(255, 255, 255));
    SetButtonColor(ui->exit_pb, QColor(30, 30, 30)); SetButtonTextColor(ui->exit_pb, QColor(255, 255, 255));
    SetButtonColor(ui->init_skybox_pb, QColor(30, 30, 30)); SetButtonTextColor(ui->init_skybox_pb, QColor(255, 255, 255));
    SetButtonColor(ui->save_pb, QColor(30, 30, 30)); SetButtonTextColor(ui->save_pb, QColor(255, 255, 255));
    SetButtonColor(ui->upload_file_pb, QColor(30, 30, 30)); SetButtonTextColor(ui->upload_file_pb, QColor(255, 255, 255));
    SetButtonColor(ui->upload_world_pb, QColor(30, 30, 30)); SetButtonTextColor(ui->upload_world_pb, QColor(255, 255, 255));
}

void menu::SetQLabelsInterface()
{
    SetQLabelTextColor(ui->label, Qt::white);
    SetQLabelTextColor(ui->label_2, Qt::white);
    SetQLabelTextColor(ui->label_3, Qt::white);
    SetQLabelTextColor(ui->label_4, Qt::white);
    SetQLabelTextColor(ui->label_5, Qt::white);
    SetQLabelTextColor(ui->label_7, Qt::white);
    SetQLabelTextColor(ui->label_8, Qt::white);
    SetQLabelTextColor(ui->label_9, Qt::white);
}

void menu::UpdateListWidget()
{
    ui->scene_lw->clear();

    static QFont font;
    font.setPixelSize(17);

    for (qsizetype i = 0; i < ui->canvas_widget->skyboxes.size(); ++i)
    {
        auto item = new QListWidgetItem(QString("skybox %1").arg(i + 1));
        item->setForeground(Qt::white);
        item->setFont(font);
        ui->scene_lw->addItem(item);
    }
}

void menu::RegisterFactories()
{
    factory.add<skybox>(DimensionalObjects::background);
    factory.add<dimensional>(DimensionalObjects::scene_object);
}

void menu::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:

        ui->canvas_widget->resize(canvas_size);
        ui->canvas_widget->move(canvas_pos);

        showNormal();

        break;

    case Qt::Key_T:

        static int skybox_index = 1;

        if (skybox_index >= ui->canvas_widget->skyboxes.size())
            skybox_index = 0;

        ui->canvas_widget->switch_skybox(skybox_index);

        ++skybox_index;

        break;
    }

    ui->canvas_widget->keyPressEvent(event);
}



void menu::on_scene_lw_itemClicked(QListWidgetItem *item)
{
    auto text = item->text();
    int skybox_index = text.size() > 7 ? QString(text.erase(text.begin(), text.begin() + 6)).toInt()
                                             : text[6].unicode();
    --skybox_index;

    ui->canvas_widget->switch_skybox(skybox_index);

    setFocusPolicy(Qt::StrongFocus);
}
