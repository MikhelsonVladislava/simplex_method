#ifndef SOLUTIONVIEW_H
#define SOLUTIONVIEW_H

#include <QWidget>
#include "../../simplex_logic/headers/units.h"
#include "../../simplex_logic/headers/task.h"
#include "../../simplex_logic/headers/table.h"
#include <memory>
#include <QLabel>
#include <QGridLayout>

namespace Ui {
class SolutionView;
class TableViewTemplate;
}

class TableViewTemplate : public QWidget
{
private:
    int vars_count = 0;
    int eqs_count = 0;
    TableState<Fraction> state;
    int rows_number = 0;
    int column_number = 0;


public:
    explicit TableViewTemplate(QWidget* parent) : QWidget(parent) {};
    TableViewTemplate(std::shared_ptr<TableState<Fraction>> curr_state, QWidget* parent = nullptr);
};

class SolutionView : public QWidget
{
    Q_OBJECT

public:
    explicit SolutionView(QWidget *parent = nullptr);
    SolutionView(std::shared_ptr<TableState<Fraction>> t, QWidget *parent = nullptr) : SolutionView(parent)
    {
        table = Table<Fraction>(t);
        create_interface();
    }
    ~SolutionView();

private:
    Ui::SolutionView *ui;
    Table<Fraction> table;
    void create_interface();
    int space_size = 15;
    int window_height = 700;
    int window_width = 1290;
};

#endif // SOLUTIONVIEW_H
