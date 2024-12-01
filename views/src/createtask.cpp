#include "../headers/createtask.h"
#include "../../ui_createtask.h"
#include<string>
#include<map>
#include <QComboBox>
#include <QScrollArea>
#include <QLabel>
#include <QPointer>
#include <QRegularExpressionValidator>
#include <QScrollArea>
#include <QObject>
#include <QAbstractButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QValidator>
#include <memory>

CreateTask::CreateTask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateTask)
{
    ui->setupUi(this);
}

CreateTask::CreateTask(int eqs_count, int var_count, QWidget *parent) :
    CreateTask(parent)
{
    this->var_count = var_count;
    this->eqs_count = eqs_count;
    create_interface();
}

void CreateTask::create_interface()
{
    QPointer<QVBoxLayout> v_layout = new QVBoxLayout(this);
    QPointer<QScrollArea> scroll_area = new QScrollArea;
    v_layout->addWidget(scroll_area);
    //scroll_area->resize(this->width() - 2, this->height() - 40);
    int common_height = eq_height + eqs_count * eq_height;
    task_view = new TaskView(var_count, eqs_count, this->width() - task_borders, common_height);
    scroll_area->setWidget(task_view);

    QPointer<QPushButton> create_task_button = new QPushButton;
    create_task_button->setText("Решить");
    v_layout->addWidget(create_task_button);
    QObject::connect(create_task_button, &QAbstractButton::clicked, this, &CreateTask::push_create_task_but);
}

CreateTask::~CreateTask()
{
    delete ui;
}

void CreateTask::push_create_task_but()
{
    Task<Fraction> t = collect_task();
    std::shared_ptr<TableState<Fraction>> start_state = std::make_shared<TableState<Fraction>>(TableState(t));
    sub_window = new SolutionView(start_state);
    sub_window->show();
}

Task<Fraction> CreateTask::collect_task()
{
    std::map<std::string, SIGN> sign_map = {{"<=", lt}, {">=", gt}, {"==", eq}};
    std::map<std::string, target> string_target = {{"max", max}, {"min", min}};
    std::vector<SystemElement<Fraction>> system_elements = {};
    for (int i = 0; i < eqs_count; i++)
    {
        std::map<int, Fraction> coeffs = {};
        for (int k = 0; k < var_count; k++)
        {
            int next_coeff = task_view->get_system_view()->get_system_c(i, k);
            coeffs[k] = Fraction(next_coeff, 1);
        }
        int right_part = task_view->get_system_view()->get_eq_right_part(i);
        std::string sign = task_view->get_system_view()->get_eq_sign(i);
        SystemElement<Fraction> next_element(coeffs, sign_map[sign], right_part);
        system_elements.push_back(next_element);
    }
    System<Fraction> system(system_elements);

    std::map<int,Fraction> tar_coeffs = {};
    for (int i = 0; i < var_count; i++)
    {
        int next = task_view->get_target_view()->get_coeff(i);
        tar_coeffs[i] = Fraction(next, 1);
    }
    std::string target_string = task_view->get_target_view()->get_target();
    TargetFunction<Fraction> target(tar_coeffs, string_target[target_string]);

    return Task(target, system);
}


