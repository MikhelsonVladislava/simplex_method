#ifndef TASKVIEW_H
#define TASKVIEW_H

#include "../../simplex_logic/headers/task.h"
#include "../../simplex_logic/headers/units.h"
#include <QComboBox>
#include <QLineEdit>
#include <QScrollArea>
#include <QWidget>
#include <QPointer>
#include <QLineEdit>

namespace Ui {
class TaskView;
}
class CoeffBox
{
private:
    int var_number = 0;
    QString name;
    QPointer<QWidget> splitter;

    void create_interface();
public:
    CoeffBox(QString name, int var_number)
    {
        this->var_number = var_number;
        this->name = name;
        splitter = new QWidget;
        create_interface();
    }

    QWidget* get_object()
    {
        return this->splitter;
    }
    int get_coeff()
    {
        return splitter->findChild<QLineEdit*>(name)->text().toInt();
    }

};


class TargetFunctionView
{
private:
    QPointer<QWidget> parent = nullptr;
    int var_count = 0;
    QPointer<QScrollArea> object = nullptr;
    std::vector<CoeffBox*> coeffs = {};

    void create_interface();
public:
    TargetFunctionView() {};
    TargetFunctionView(int var_count, QWidget* parent)
    {
        this->var_count = var_count;
        this->parent = parent;
        create_interface();
    }
    int get_coeff(int var_number) {return coeffs[var_number]->get_coeff();}
    QScrollArea* get_object()
    {
        return object;
    }
};


class EqSystem
{
private:
    int var_count = 0;
    std::vector<CoeffBox*> coeffs = {};
    QPointer<QComboBox> sign_box = nullptr;
    QPointer<QLineEdit> right_part_window = nullptr;
    QPointer<QScrollArea> object = nullptr;

    void create_interface();
public:
    EqSystem() {};
    EqSystem(int var_count) {
        this->var_count = var_count;
        create_interface();}
    void resize(int w, int h)
    {object->resize(w, h);}
    QScrollArea* get_object() {return object;};

};

class SystemView
{
private:
    int var_count = 0;
    int eqs_count = 0;
    std::vector<EqSystem*> eqs = {};
    QPointer<QWidget> parent;
    QPointer<QWidget> object = nullptr;

    void create_interface();

public:
    SystemView() {};
    SystemView(int var_count, int eqs_count, QWidget* parent = nullptr) {
        this->var_count = var_count;
        this->eqs_count = eqs_count;
        this->parent = parent;

        create_interface();
    }
    void resize(int w, int h);
    QWidget* get_object() {return object;};
};



class TaskView : public QWidget
{
    Q_OBJECT

public:
    explicit TaskView(QWidget *parent = nullptr);
    TaskView(int var_count, int eqs_count, QWidget *parent = nullptr) :
        TaskView(parent)
    {
        this->var_count = var_count;
        this->eqs_count = eqs_count;
        create_interface();
    };
    void resize_all();
//    QPointer<QWidget> get_target_interface_element();
//    QPointer<QWidget> get_system_inderface_element();
    ~TaskView();

private:
    SystemView system_view;
    TargetFunctionView target_view;
    Ui::TaskView *ui;
    int var_count = 0;
    int eqs_count = 0;
    int target_view_height = 60;
    int layout_space = 50;
    int system_eqheight = 60;
    int system_eqwidth = 0;
    int target_view_width = 0;

    void create_interface();
};

#endif // TASKVIEW_H
