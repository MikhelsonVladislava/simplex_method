#include "../headers/taskview.h"
#include "../../ui_taskview.h"
#include <QSplitter>
#include <QPointer>
#include <QLineEdit>
#include <QComboBox>
#include <QValidator>
#include <QScrollArea>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>

TaskView::TaskView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskView)
{
    ui->setupUi(this);
}

void TaskView::resize_all()
{
    int common_height = target_view_height + layout_space + eqs_count * 100;
    this->resize(this->parentWidget()->width() - 60, common_height);
}

void TaskView::create_interface()
{
    QPointer<QVBoxLayout> layout = new QVBoxLayout(this);
    layout->setSpacing(layout_space);

    target_view = TargetFunctionView(var_count, this);
    QPointer<QScrollArea> target_sc= target_view.get_object();
    layout->addWidget(target_sc);
    system_view = SystemView(var_count, eqs_count, this);
    QPointer<QWidget> system_object = system_view.get_object();
    layout->addWidget(system_view.get_object());
    layout->setStretchFactor(target_view.get_object(), 1);
    layout->setStretchFactor(system_view.get_object(), eqs_count);
    resize_all();
}

TaskView::~TaskView()
{
    delete ui;
}

void CoeffBox::create_interface()
{
    QRegularExpression rx("-?[0-9]*");
    QPointer<QValidator> validator = new QRegularExpressionValidator(rx, splitter);

    QPointer<QHBoxLayout> coeff_var_widget = new QHBoxLayout(splitter);
    QPointer<QLineEdit> coeff_window = new QLineEdit;
    coeff_window->setAccessibleName(name);
    coeff_var_widget->addWidget(coeff_window);
    coeff_window->setValidator( validator );

    QPointer<QLabel> var = new QLabel;
    coeff_var_widget->addWidget(var);
    QString var_label_text = "*X" + QString::number(var_number);
    var->setText(var_label_text);


}

void TargetFunctionView::create_interface()
{
    object = new QScrollArea(parent);
    QPointer<QWidget> target = new QWidget(object);
    QPointer<QHBoxLayout> layout = new QHBoxLayout(target);
    for (int i = 0; i < var_count; i++)
    {
        CoeffBox* coeff_var= new CoeffBox("target_function_coeff" + QString::number(i), i);
        layout->addWidget(coeff_var->get_object());
        coeffs.push_back(coeff_var);
        if (i < var_count - 1)
        {
            QPointer<QLabel> plus_label = new QLabel;
            layout->addWidget(plus_label);
            QString var_label_text = QString(" + ");
            plus_label->setText(var_label_text);
        }

    }

    QPointer<QLabel> arrow = new QLabel;
    layout->addWidget(arrow);
    QString target_label_text = QString(" --> ");
    arrow->setText(target_label_text );


    QPointer<QComboBox> target_combo_box = new QComboBox;
    QStringList box_variants = (QStringList() << "max" << "min");
    target_combo_box->addItems(box_variants);
    layout ->addWidget(target_combo_box);
    object->setWidget(target);
}

void SystemView::create_interface()
{
    object = new QWidget(parent);
    QPointer<QVBoxLayout> layout = new QVBoxLayout(object);

    for (int j = 0; j < eqs_count; j++)
    {
        EqSystem* eq = new EqSystem(var_count); //
        eqs.push_back(eq);
        layout->addWidget(eq->get_object());
    }
}

void SystemView::resize(int w, int h)
{
    for (int k = 0; k < eqs_count; k++)
    {
        eqs[k]->resize(w, h);
    }
}

void EqSystem::create_interface()
{

    QStringList sign_box_variants = (QStringList() << "<=" << "==" << ">=");
    object = new QScrollArea;
    QPointer<QWidget> container = new QWidget;
    QRegularExpression rx("-?[0-9]*");
    QPointer<QValidator> validator = new QRegularExpressionValidator(rx, container);
    QPointer<QHBoxLayout> layout = new QHBoxLayout(container);

    for (int j = 0; j < var_count; j++)
    {
        CoeffBox* var_coeff = new CoeffBox("eq_function_coeff" + QString::number(j), j);
        coeffs.push_back(var_coeff);
        layout->addWidget(var_coeff->get_object());

        if (j < var_count - 1)
        {
            QPointer<QLabel> plus = new QLabel;
            QString var_label_text = " + ";
            plus->setText(var_label_text);
            layout->addWidget(plus);
        }
    }
    QPointer<QComboBox> next_sign_combo_box = new QComboBox;
    next_sign_combo_box->addItems(sign_box_variants);
    layout->addWidget(next_sign_combo_box);
    right_part_window = new QLineEdit;
    right_part_window->setAccessibleName("right_part");
    right_part_window->setValidator( validator );
    layout->addWidget(right_part_window);
    sign_box = next_sign_combo_box;
    object->setWidget(container);
}
