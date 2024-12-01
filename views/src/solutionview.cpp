#include "../headers/solutionview.h"
#include "../../ui_solutionview.h"
#include <QPointer>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>

SolutionView::SolutionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolutionView)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

SolutionView::~SolutionView()
{
    delete ui;
}

void SolutionView::create_interface()
{
    this->resize(window_width, window_height);
    QPointer<QVBoxLayout> main_layout = new QVBoxLayout(this);

    table.optimize();
    std::vector<std::shared_ptr<TableState<Fraction>>> hist = table.get_history();
    int iterations_count = hist.size();

    QPointer<QScrollArea> sol_history_scroll_area = new QScrollArea(this);
    QPointer<QWidget> container = new QWidget;
    sol_history_scroll_area->setWidget(container);
    QPointer<QVBoxLayout> layout = new QVBoxLayout(container);
    main_layout->addWidget(sol_history_scroll_area);
    layout->setSpacing(space_size);

    int max_state_height = 0;
    for (int j = 0; j < iterations_count; j++)
    {
        QPointer<TableViewTemplate> table_state = new TableViewTemplate(hist[j]);
        layout->addWidget(table_state);
        if (table_state->height() > max_state_height)
            max_state_height = table_state->height();
        table_state->setStyleSheet("QWidget { background-color : black; } QLabel { font-size: 40px; background-color : white; color : black; } ");

    }
    QPointer<QWidget> result_window = new QWidget;
    QPointer<QHBoxLayout> layout_result_window = new QHBoxLayout(result_window);
    result_window->setStyleSheet("QLabel { font-size: 40px; background-color : white; color : black; } ");

    QPointer<QLabel> opt_plan_label = new QLabel;
    QString opt_plan_txt = "(";
    std::vector<Fraction> op_vector = table.optimum();
    int n = op_vector.size();
    for (int k = 0; k < n; k++)
    {
        opt_plan_txt = opt_plan_txt + QString::fromStdString(op_vector[k].to_string());
        if (k < n - 1) opt_plan_txt += QString(", ");
    }
    opt_plan_txt = opt_plan_txt + QString(")");
    opt_plan_label->setText(opt_plan_txt);
    layout_result_window->addWidget(opt_plan_label);

    QPointer<QLabel> f_value = new QLabel;
    opt_plan_txt = "F = " + QString::fromStdString(table.get_tagret_value().to_string());
    f_value->setText(opt_plan_txt);
    layout_result_window->addWidget(f_value);
    layout_result_window->setSpacing(space_size);

    layout_result_window->setAlignment(Qt::AlignCenter);

    main_layout->setAlignment(Qt::AlignCenter);
    main_layout->addWidget(result_window);
    sol_history_scroll_area->resize(this->width(), this->height() / 1.5);
    int h = iterations_count * (max_state_height + space_size);
    int w = this->width();
    container->resize(w,h);
}

TableViewTemplate::TableViewTemplate(std::shared_ptr<TableState<Fraction> > curr_state, QWidget *parent) : TableViewTemplate(parent)
{
    state = TableState(curr_state);
    vars_count = state.get_target_coeffs_c().size();
    eqs_count = state.get_basis().size();

    rows_number = eqs_count + 3;
    column_number = vars_count + 3;

    QGridLayout* grid = new QGridLayout(this);

    QLabel* i_label = new QLabel(QString("i"));
    grid->addWidget(i_label, 0, 0, Qt::AlignCenter);
    QLabel* right_part_label = new QLabel("b");
    grid->addWidget(right_part_label, 1, 2, Qt::AlignCenter);

    QLabel* basis_label = new QLabel(QString("basis"));
    grid->addWidget(basis_label, 1, 1, Qt::AlignCenter);

    for (int j = 0; j < vars_count; j++)
    {

        QLabel* basis_ind_label = new QLabel(QString("P") + QString::number(j));
        grid->addWidget(basis_ind_label, 1, 3 + j, Qt::AlignCenter);

        std::string txt_str = state.get_target_coeff_c(j).to_string();
        QString txt = QString::fromStdString(txt_str);
        QLabel* targetc = new QLabel(txt);
        grid->addWidget(targetc, 0, 3 + j, Qt::AlignCenter);

        txt_str = state.get_target_f_eq(j).to_string();
        txt = QString::fromStdString(txt_str);
        QLabel* target_f = new QLabel(txt);
        grid->addWidget(target_f, rows_number - 1, 3 + j, Qt::AlignCenter);
    }

    for (int i = 0; i < eqs_count; i++)
    {
        QLabel* num_label = new QLabel(QString::number(i));
        grid->addWidget(num_label, 2 + i, 0, Qt::AlignCenter);


        QLabel* basis_num_label = new QLabel(QString::number(state.get_basis()[i]));
        grid->addWidget(basis_num_label, 2 + i, 1, Qt::AlignCenter);

        std::string txt_str = state.get_right_part_eq(i).to_string();
        QString txt = QString::fromStdString(txt_str);
        QLabel* right = new QLabel(txt);
        grid->addWidget(right, i+2, 2, Qt::AlignCenter);

        for (int j = 0; j < vars_count; j++)
        {
            txt_str = state.get_main_table_coeff_var(i,j).to_string();
            txt = QString::fromStdString(txt_str);
            QLabel* mainc = new QLabel(txt);
            grid->addWidget(mainc, i + 2, 3 + j, Qt::AlignCenter);
        }
    }


}



