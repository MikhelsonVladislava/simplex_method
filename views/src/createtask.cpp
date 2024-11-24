#include "../headers/createtask.h"
#include "../../ui_createtask.h"

CreateTask::CreateTask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateTask)
{
    ui->setupUi(this);
}

void CreateTask::create_interface()
{

}

CreateTask::~CreateTask()
{
    delete ui;
}
