#ifndef TABLE_H
#define TABLE_H
#include "task.h"
#include <iomanip>
#include <iostream>
// ограничение на номера переменных
template<typename maintype>
class TableState
{
private:
    std::map<int, int> basis = {};
    std::map<int, maintype> target_coeffs_c = {};
    std::map<int, maintype> right_part_b = {};
    std::vector<std::vector<maintype>> main_table_coeffs = {};
    std::map<int, maintype> target_f = {};
    int system_var_count = 0;
    int system_elements_count = 0;

    maintype calculate_target_f(int var_number)
    {
        maintype z = 0; //
        
        for (int i = 0; i < system_elements_count; i++)
                z += target_coeffs_c[basis[i]] * main_table_coeffs[i][var_number];

        return z - target_coeffs_c[var_number];
    }

    void make_basis() 
    // требует наличия target_coeffs_c, main_table_coeffs потому что меняет их
    {

        for (int i = 0; i < system_var_count; i++)
        {
            int basic_var_eq_no_zero = -1;
            bool coeff_is_one = false;
            for (int j = 0; j < system_elements_count; j++)
                if (main_table_coeffs[j][i] == 1 && !coeff_is_one) 
                {
                    basic_var_eq_no_zero = j;
                    coeff_is_one = true;
                } else if(main_table_coeffs[j][i] != 0 && coeff_is_one)
                {
                    basic_var_eq_no_zero = -1;
                    break;
                }

            if (basic_var_eq_no_zero != -1)
                basis[basic_var_eq_no_zero] = i;
        }

        //добавляем переменные
        int basis_vars_count = basis.size() - 1;
        int basics_vars_residue = system_elements_count - basis_vars_count;
        if (basics_vars_residue > 0)
        {
            for (int j = 1; j < system_elements_count - basis_vars_count; j++)
            {
                bool new_basic_var_is_choose = false;
                for (int k = 0; k < system_elements_count; k++)
                {
                    std::vector<maintype>* current_eq = &main_table_coeffs[k];
                    if (!basis.count(k) && !new_basic_var_is_choose)
                    {
                        current_eq->push_back(1); //
                        basis[k] = system_var_count;
                        new_basic_var_is_choose = true;
                    }
                    else
                        current_eq->push_back(0); //
                    target_coeffs_c[system_var_count] = 0; //
                }
                
                system_var_count += 1;
            }
        }
    }

    int choose_main_col()
    {
        // обработка оптимизирован
        maintype min_coeff = 0; 
        int main_col = -1;
        for (int i = 0; i < system_var_count; i++)
        {
            if (target_f[i] < min_coeff) {
                main_col = i;
                min_coeff = target_f[i];
            }
        }

        return main_col;
    }


public:
    TableState(const Task<maintype>& task)
    {
        System<maintype> system = task.get_system();
        system_elements_count = system.size();
        system_var_count = system[0]->size();

        for (int j = 0; j < system_elements_count; j++)
        {
            main_table_coeffs.push_back({});
            for (int k = 0; k < system_var_count; k++)
                main_table_coeffs[j].push_back((*system[j])[k]);
        }   

        target_coeffs_c = task.get_target().get_coeffs_map();
        right_part_b = system.get_right_part();
        make_basis();

        for (int i = 0; i < system_var_count; i++)
            target_f[i] = calculate_target_f(i);
    }

    TableState(const TableState<maintype>& last_state)
    {
        basis = last_state.basis;
        target_coeffs_c = last_state.target_coeffs_c;
        right_part_b = last_state.right_part_b;
        main_table_coeffs = last_state.main_table_coeffs;
        target_f = last_state.target_f;
        system_var_count = last_state.system_var_count;
        system_elements_count = last_state.system_elements_count;
    }
    
    std::map<int, maintype> get_target_coeffs_c() {return target_coeffs_c;}

    bool is_optimized()
    {
        for (int i = 0; i < system_var_count; i++)
            if (target_f[i] < 0) return false;
        return true;
    }

    void optimize()
    {
        std::vector<maintype> choose_main_row_vector = {};
        int main_column = choose_main_col();
        for (int k = 0; k < system_elements_count; k++)
            choose_main_row_vector.push_back(right_part_b[k] / main_table_coeffs[k][main_column]);
        
        maintype main_row_coeff = choose_main_row_vector[0];
        int main_row = 0;
        for (int i = 1; i < system_elements_count; i++)
        {
            if ((main_row_coeff < 0 && choose_main_row_vector[i] >= 0) ||
                    (choose_main_row_vector[i] >= 0 && choose_main_row_vector[i] < main_row_coeff))
            {
                main_row_coeff = choose_main_row_vector[i];
                main_row = i;
            }
        }
        basis[main_row] = main_column;
        
        maintype last_main = main_table_coeffs[main_row][main_column];
        for (int i = 0; i < system_var_count; i++)
            main_table_coeffs[main_row][i] /= last_main;
        right_part_b[main_row] /= last_main;

        for (int j = 0; j < system_elements_count; j++)
        {
            maintype last_state_row_main_column = main_table_coeffs[j][main_column];
            maintype sub_value = last_state_row_main_column * right_part_b[main_row];
            
            if (j != main_row)
            {
                right_part_b[j] -= sub_value;
                for (int i = 0; i < system_var_count; i++)
                {
                    sub_value = last_state_row_main_column * main_table_coeffs[main_row][i];
                    main_table_coeffs[j][i] -= sub_value;        
                }

            }
        }

        for (int j = 0; j < system_var_count; j++)
            target_f[j] = calculate_target_f(j);
    }

    int get_var_number() {return system_var_count;}
    std::map<int, int> get_basis() {return basis;}
    std::map<int, maintype> right_part() {return right_part_b;}

    void view()
    {
        std::cout << std::setw(4) << " " << std::setw(4) << " " << std::setw(4) << " ";
        for (int i = 0; i < system_var_count; i++)
            std::cout << std::setw(4) << target_coeffs_c[i];
        std::cout << std::endl;
        for (int k = 0; k < system_elements_count; k++)
        {
            std::cout << std::setw(4) << k << std::setw(4) << basis[k] << std::setw(4) << right_part_b[k];
            for (int j = 0; j < system_var_count; j++)
                std::cout << std::setw(4) << main_table_coeffs[k][j];
            std::cout << std::endl;
        }

        std::cout << std::setw(4) << " " << std::setw(4) << " " << std::setw(4) << " ";
        for (int i = 0; i < system_var_count; i++)
            std::cout << std::setw(4) << target_f[i];

    }

};

template<typename maintype>
class Table
{
private:
    int iteration = 0;
    TableState<maintype>* current_state;

public:
    Table(TableState<maintype>* start_state)
    {
        current_state = start_state;
    }

    void optimize()
    {
        current_state->view();
        std::cout << std:: endl << "\n";
        while(!current_state->is_optimized())
        {
            current_state->optimize();
            current_state->view();
            iteration++;
            std::cout << std:: endl << "\n";
        }
    }

    maintype get_tagret_value()
    {
        maintype result = 0;
        int vars_num = current_state->get_var_number();
        std::map<int, maintype> coeffs = current_state->get_target_coeffs_c();
        std::vector<maintype> optimum_plan = optimum();
        for (int i = 0; i < vars_num; i++)
        {
            result += coeffs[i] * optimum_plan[i];
        }

        return result;
    }

    std::map<int, int> get_basis_number_eq_number()
    {
        std::map<int, int> resul = {};
        std::map<int, int> vars_numbers = current_state->get_basis();
        for (const auto& [k, v] : vars_numbers)
        {
            resul[v] = k;
        }
        return resul;
    }

    std::vector<maintype> optimum()
    {
        std::map<int, int> vars_nums = get_basis_number_eq_number();
        std::map<int, maintype> right_p = current_state->right_part();
        int vars_num = current_state->get_var_number();
        std::vector<maintype> resul = {};
        for (int i = 0; i < vars_num; i++)
        {
            if (vars_nums.count(i)) resul.push_back(right_p[vars_nums[i]]);
            else resul.push_back(0);
        }
        return resul;
    }
};

#endif
