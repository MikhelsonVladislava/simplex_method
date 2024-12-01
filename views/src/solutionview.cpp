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
    ui->setupUi(this);
}

SolutionView::~SolutionView()
{
    delete ui;
}

void SolutionView::create_interface()
{
    table.optimize();
    std::vector<std::shared_ptr<TableState<Fraction>>> hist = table.get_history();
    int iterations = hist.size();
    QPointer<QScrollArea> area = new QScrollArea(this);
    QPointer<QWidget> container = new QWidget(area);
    QPointer<QVBoxLayout> layout = new QVBoxLayout(container);
    for (int j = 0; j < iterations; j++)
    {
        QPointer<TableViewTemplate> table_state = new TableViewTemplate(hist[j]);
        layout->addWidget(table_state);
    }
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
    grid->addWidget(i_label, 0, 0);
    QLabel* basis_label = new QLabel(QString("basis"));
    grid->addWidget(basis_label, 1, 1);
    for (int j = 0; j < vars_count; j++)
    {

        QLabel* basis_ind_label = new QLabel(QString(" ") + QString::number(j));
        grid->addWidget(basis_ind_label, 1, 3 + j);

        std::string txt_str = state.get_target_coeff_c(j).to_string();
        QString txt = QString::fromStdString(txt_str);
        QLabel* targetc = new QLabel(txt);
        grid->addWidget(targetc, 0, 3 + j);

        txt_str = state.get_target_f_eq(j).to_string();
        txt = QString::fromStdString(txt_str);
        QLabel* target_f = new QLabel(txt);
        grid->addWidget(target_f, rows_number - 1, 3 + j);
    }

    for (int i = 0; i < eqs_count; i++)
    {
        QLabel* num_label = new QLabel(QString::number(i));
        grid->addWidget(num_label, 2 + i, 0);

        QLabel* basis_num_label = new QLabel(QString::number(state.get_basis()[i]));
        grid->addWidget(basis_num_label, 2 + i, 1);
        std::string txt_str = state.get_right_part_eq(i).to_string();
        QString txt = QString::fromStdString(txt_str);
        QLabel* right = new QLabel(txt);
        grid->addWidget(right, i+2, 2);
        for (int j = 0; j < vars_count; j++)
        {
            txt_str = state.get_main_table_coeff_var(i,j).to_string();
            txt = QString::fromStdString(txt_str);
            QLabel* mainc = new QLabel(txt);
            grid->addWidget(mainc, i + 2, 3 + j);
        }
    }


}



