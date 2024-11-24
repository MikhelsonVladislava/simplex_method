#include <iostream>
#include "../headers/task.h"
#include <map>
#include "../headers/units.h"
#include "../headers/table.h"

int main()
{
    SystemElement<Fraction> el1 = SystemElement<Fraction>({{0,Fraction(1, 1)},{1,Fraction(1, 1)},{2,Fraction(1, 1)}, {3,Fraction(0,1)}, {4,Fraction(0,1)}}, eq, 5);
    SystemElement<Fraction> el2 = SystemElement<Fraction>({{0,Fraction(2, 1)},{1,Fraction(1, 1)},{2,0}, {3,Fraction(1, 1)}, {4,0}}, eq, 9);
    SystemElement<Fraction> el3 = SystemElement<Fraction>({{0,Fraction(1, 1)},{1,Fraction(2, 1)},{2,0}, {3,0}, {4,Fraction(1, 1)}}, eq, 7);

    System<Fraction> s = System<Fraction>({el1, el2, el3});

    TargetFunction<Fraction> t = TargetFunction<Fraction>({{0,2},{1,1},{2,-1}, {3,1}, {4,-1}}, max);
    Task<Fraction> t1 = Task<Fraction>(t, s);

    TableState<Fraction> s1 = TableState(t1);
    Table<Fraction> t2 = Table(&s1);
    t2.optimize();


    std::cout << t2.get_tagret_value();
}
