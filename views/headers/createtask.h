#ifndef CREATETASK_H
#define CREATETASK_H

#include <QScrollArea>
#include <QWidget>
#include <QPointer>
#include "../../simplex_logic/headers/task.h"
#include "../../simplex_logic/headers/table.h"
#include "solutionview.h"
#include "../../simplex_logic/headers/units.h"
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
    CreateTask(int eqs_count, int var_count, QWidget *parent = nullptr);

    void create_interface();

    ~CreateTask();

public slots:
    void push_create_task_but();

private:
    Ui::CreateTask *ui;
    QPointer<TaskView> task_view = nullptr;
    QPointer<SolutionView> sub_window = nullptr;

    int eq_height = 100;
    int task_borders = 100;

    int eqs_count = 0;
    int var_count = 0;

    Task<Fraction> collect_task();
};

#endif // CREATETASK_H
