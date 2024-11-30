#ifndef UNITS_H
#define UNITS_H
#include <vector>
#include <iostream>
#include <math.h>

class Fraction
{
private:
    int numerator = 0;
    int denominator = 1;

    void reduce();

public:
    Fraction() {};
    Fraction(int numerator, int denominator);
    Fraction operator+(const Fraction& sum);
    void operator+=(const Fraction& sum);
    Fraction operator*(const Fraction& mul);
    void operator*=(const Fraction& mul);
    Fraction operator-(const Fraction& sub);
    void operator-=(const Fraction& sub);
    Fraction operator/(const Fraction& div);
    void operator/=(const Fraction& div);

    
    int operator==(const Fraction& second);
    int operator!=(const Fraction& second);
    int operator>(const Fraction& second);
    int operator<(const Fraction& second);
    int operator>=(const Fraction& second);
    int operator<=(const Fraction& second);

    Fraction(int number);

    int get_denominator() const;
    int get_numerator() const;

};

#endif
