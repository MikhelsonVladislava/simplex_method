#ifndef TASKVIEW_H
#define TASKVIEW_H

#include "../../simplex_logic/headers/task.h"
#include "../../simplex_logic/headers/units.h"
#include<string>
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
    QPointer<QLineEdit> coeff_window = nullptr;
    QPointer<QWidget> splitter = nullptr;

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
        return coeff_window->text().toInt();
    }

};


class TargetFunctionView
{
private:
    QPointer<QWidget> parent = nullptr;
    int var_count = 0;
    QPointer<QScrollArea> object = nullptr;
    QPointer<QComboBox> target_box= nullptr;
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
    std::string get_target() {return target_box->currentText().toStdString();}
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
    int get_right_part() {return right_part_window->text().toInt();}
    std::string get_sign_str() {return sign_box->currentText().toStdString();}
    CoeffBox* get_coeff_box(int i) {return coeffs[i];}
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
    EqSystem* get_eq(int j) {return eqs[j];}
    int get_system_c(int row, int col);
    std::string get_eq_sign(int i) {return eqs[i]->get_sign_str();}
    int get_eq_right_part(int i) {return eqs[i]->get_right_part();}
};



class TaskView : public QWidget
{
    Q_OBJECT

public:
    explicit TaskView(QWidget *parent = nullptr);
    TaskView(int var_count, int eqs_count, int w, int h, QWidget *parent = nullptr) :
        TaskView(parent)
    {
        this->var_count = var_count;
        this->eqs_count = eqs_count;
        this->resize(w, h);
        create_interface();
    };
    SystemView* get_system_view() {return &system_view;}
    TargetFunctionView* get_target_view() {return &target_view;}
//    QPointer<QWidget> get_target_interface_element();
//    QPointer<QWidget> get_system_inderface_element();
    ~TaskView();

private:
    SystemView system_view;
    TargetFunctionView target_view;
    Ui::TaskView *ui;
    int var_count = 0;
    int eqs_count = 0;
    int layout_space = 50;

    void create_interface();
};

#endif // TASKVIEW_H
