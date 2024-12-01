#include "../headers/mainwindow.h"
#include "../headers/createtask.h"
#include <QSpinBox>
#include "../../ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_create_but_clicked()
{
    QSpinBox* eqs_count = findChild<QSpinBox*>("eqs_count");
    QSpinBox* var_count = findChild<QSpinBox*>("var_count");

    if (sub_window == nullptr)
    {
        int eqs_count_int = eqs_count->value();
        int var_count_int = var_count->value();

        CreateTask* create_task = new CreateTask(eqs_count_int, var_count_int);
        sub_window = create_task;
        sub_window->show();
    }
}

