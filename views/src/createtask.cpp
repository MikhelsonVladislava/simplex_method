#include "../headers/createtask.h"
#include "../../ui_createtask.h"
#include <QComboBox>
#include <QScrollArea>
#include <QLabel>
#include <QPointer>
#include <QRegularExpressionValidator>
#include <QScrollArea>
#include <QLineEdit>

#include <QValidator>

CreateTask::CreateTask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateTask)
{
    ui->setupUi(this);
}

void CreateTask::create_interface()
{
    QPointer<QScrollArea> scroll_area = new QScrollArea(this);
//    QPointer<QLabel> label = new QLabel(scroll_area);
//    label->setText("awddwaw");
    scroll_area->resize(this->width() - 2, this->height() - 40);
    task_view = new TaskView(var_count, eqs_count, this);
    scroll_area->setWidget(task_view);
    task_view->show();

}

CreateTask::~CreateTask()
{
    delete ui;
}


