#ifndef CREATETASK_H
#define CREATETASK_H

#include <QScrollArea>
#include <QWidget>
#include <QPointer>
#include "taskview.h"
#include <QLineEdit>


namespace Ui {
class CreateTask;
}
class CreateTask : public QWidget
{
    Q_OBJECT

public:
    explicit CreateTask(QWidget *parent = nullptr);
    CreateTask(int eqs_count, int var_count, QWidget *parent = nullptr) :
        CreateTask(parent)
    {
        this->var_count = var_count;
        this->eqs_count = eqs_count;
        create_interface();
    }

    void create_interface();

    ~CreateTask();

private:
    Ui::CreateTask *ui;
    QPointer<TaskView> task_view = nullptr;

    int eqs_count = 0;
    int var_count = 0;
};

#endif // CREATETASK_H
