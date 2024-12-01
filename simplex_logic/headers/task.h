#ifndef TASK_H
#define TASK_H
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <map>
#include <string_view>

using std::cin;
using std::cout;

namespace simplex_exceptions
{
    class VariableMissingException
    {
    public:
        std::string_view what() const {
            return "В целевой функции и ограничении должны присутствовать все переменные от 0 до n - 1";
        }
    };

};

enum SIGN {lt, gt, eq, lts, gts};
enum target {min, max};

template<typename system_type>
class SystemElement
{
private:
    std::map<int, system_type> coeffs_vars = {};
    SIGN sign;
    system_type right_part = 0; //
    int max_var_number = 0;

public:
    SystemElement(std::map<int, system_type> coeffs_vars, SIGN sign, system_type right_part)
    {
        max_var_number = coeffs_vars.size() - 1;
        for (int j = 0; j < max_var_number; j++)
            if (coeffs_vars.count(j) == 0) throw simplex_exceptions::VariableMissingException();
        
        this->coeffs_vars = coeffs_vars;
        this->sign = sign;
        this->right_part = right_part;
    }

    SIGN get_sign() const {return (this->sign);};
    system_type operator[](int var_number) const {return coeffs_vars.at(var_number);};
    system_type get_right_part() const {return right_part;};
    int size() const {return coeffs_vars.size();};
    int get_max_var_number() const {return this->max_var_number;};

    void set_right_part(system_type right_part) {this->right_part = right_part;};
    void set_coeff(int var_number, system_type value) {coeffs_vars[var_number] = value;};
    void add_coeff(system_type coeff, int var_number) 
    {
        coeffs_vars[var_number] = coeff;
        max_var_number = max_var_number < var_number ? var_number : max_var_number;
    };
    void set_sign(SIGN sign) {this->sign = sign;};

};

template<typename system_type>
class System
{
private:
    std::vector<SystemElement<system_type>> system = {};

public:
    System() {};
    System(std::vector<SystemElement<system_type>> system) {
        this->system = system;
    }

    int size() const {return system.size();};
    
    std::map<int, system_type> get_right_part()
    {
        std::map<int, system_type> result = {};
        int size = system.size();
        for (int k = 0; k < size; k++)
            result[k] = system[k].get_right_part();
        return result;
    }
    const SystemElement<system_type>* operator[](int i) const 
    {
        const SystemElement<system_type>* system_element_link = &system.at(i);
        return system_element_link;};
    std::vector<SystemElement<system_type>> get_system() const {return system;};

    void to_basic()
    {
        // Добавить обработку на системы с неравным количеством элементов
        int n = this->size();
        int max_var_number = this->system[0].size() - 1;
        SystemElement<system_type>* system_unit = nullptr;

        for(int i = 0; i < n; i++)
        {
            system_unit = &system[i];
            
            if (system_unit->get_sign() == lt) 
            {
                max_var_number += 1;
                system_unit->add_coeff(1, max_var_number); //
            }
            else if (system_unit->get_sign() == gt)
            { 
                max_var_number += 1;
                system_unit->add_coeff(-1, max_var_number); //
            }

            if (system_unit->get_sign() != eq)
            {
                for (int j = 0; j < n; j++)
                    if (j != i) system[j].add_coeff(0, max_var_number); //
            }
            system_unit->set_sign(eq);
        }
    }
};

template<typename system_type>
class TargetFunction
{
private:
    std::map<int, system_type> coeffs_vars;
    target task_target;

public:
    TargetFunction() {};
    TargetFunction(std::map<int, system_type> coeffs_vars, target task_target)
    {
        this->task_target = task_target;
        this->coeffs_vars = coeffs_vars;
        int max_var_number = coeffs_vars.size() - 1;
        for (int j = 0; j < max_var_number; j++)
            if (coeffs_vars.count(j) == 0) throw simplex_exceptions::VariableMissingException();
    }

    int size() const {return coeffs_vars.size();};

    system_type get_coeff(int var_numder) const {return coeffs_vars.at(var_numder);};
    target get_target() const {return this->task_target;};
    system_type operator[](int var_number) const {return coeffs_vars.at(var_number);};
    std::map<int, system_type> get_coeffs_map() {return coeffs_vars;};

    //добавить обработку ошибок
    void set_coeff(system_type new_value, int var_numder) {this->coeffs_vars[var_numder] = new_value;};
    void set_target(target new_target) {this->task_target = new_target;};

    void add_var(int var_number, system_type coeff){this->coeffs_vars[var_number] = coeff;};
};

template<typename system_type>
class Task
{
private:
    System<system_type> system;
    TargetFunction<system_type> targ_func;
public:
    Task(TargetFunction<system_type> target, System<system_type> system)
    {
        this->system = system;
        this->targ_func = target;
    }

    System<system_type> get_system() const {return this->system;};
    TargetFunction<system_type> get_target() const {return this->targ_func;};

    void to_basic()
    {
        system.to_basic();
        int n = system.size();
        int m = system[0]->size();
        int target_var_max_num = targ_func.size() - 1;
        if (targ_func.get_target() == min)
        {
            targ_func.set_target(max);
            for (int j =0; j <= target_var_max_num; j++)
            {
                system_type curr_value = targ_func.get_coeff(j);
                targ_func.set_coeff(-1 * curr_value, j);
            }
        }
        std::map<int, system_type> vars_coeffs_map = targ_func.get_coeffs_map();
        if (target_var_max_num < m)
        {
            for (int j = target_var_max_num + 1; j < m; j++)
                targ_func.set_coeff(0, j);
        }
    }

};

namespace basic_view {
    
    template<typename system_type>
    void view_function(const TargetFunction<system_type>& func)
    {
        int max_num = func.size() - 1;
        
        for (int j = 0; j < max_num; j++)
        {
            system_type curr = func[j];
            if (curr < 0) cout << func.get_coeff(j) << "*x" << j;
            if (j > 0 && curr >= 0) cout << "+" << func.get_coeff(j) << "*x" << j;
        }

        cout << " --> ";
        if (func.get_target() == min) cout << "min";
        else if (func.get_target() == max) cout << "max";

        cout << "\n";
    }

    template<typename system_type>
    void view_system(const System<system_type>& system)
    {
        int n = system.size();
        int m = system[0]->size();

        for (int i = 0; i < n; i++)
        {
            const SystemElement<system_type>* current = system[i];
            cout << std::endl;
            for (int j = 0; j < m; j++)
            {
                cout << (*current)[j] << "*x" << j << " ";
            }

            SIGN sign = current->get_sign();
            if (sign == eq)
                cout << "== ";
            else if (sign == gt)
                cout << ">= ";
            else if (sign == lt)
                cout << "<= ";

            cout << current->get_right_part();
        }
        
    }
    template<typename system_type>
    void view_task(Task<system_type>& task)
    {
        TargetFunction<system_type> target= task.get_target();
        System<system_type> system = task.get_system();
        basic_view::view_function<system_type>(target);
        basic_view::view_system<system_type>(system);
    }

};
 
#endif
