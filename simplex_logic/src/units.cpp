#include "../headers/units.h"
#include <cmath>

void Fraction::reduce()
{
    int n_sqrt = std::sqrt(numerator);
    int divis = 2;

    while (divis <= numerator && divis <= denominator)
    {
        if (denominator % divis == 0 && numerator % divis == 0)
        {
            denominator /= divis;
            numerator /= divis;
        } else divis += 1;
    }

    if (numerator == 0) denominator = 1;
}

Fraction::Fraction(int numerator, int denominator)
{
    this->denominator = denominator;
    this->numerator = numerator;
} 

Fraction Fraction::operator+(const Fraction& sum) 
{
    int new_numerator = numerator * sum.denominator + sum.numerator * denominator;
    int new_denominator = denominator * sum.denominator;
    Fraction result = Fraction(new_numerator, new_denominator);
    result.reduce();
    return result;
}

void Fraction::operator+=(const Fraction& sum) 
{
    *this = *this + sum;
    this->reduce();
} 

Fraction Fraction::operator*(const Fraction& mul) 
{
    int new_numerator = numerator * mul.numerator;
    int new_denominator = denominator * mul.denominator;
    Fraction result = Fraction(new_numerator, new_denominator);
    result.reduce();
    return result;
}

void Fraction::operator*=(const Fraction& mul) 
{
    *this = *this * mul;
    this->reduce();
}

Fraction Fraction::operator-(const Fraction& sub) 
{
    Fraction result = *this + Fraction(-1, 1) * sub;
    result.reduce();
    return result;
}

void Fraction::operator-=(const Fraction& sub) 
{
    *this = *this - sub;
    this->reduce();
}

Fraction Fraction::operator/(const Fraction& div) 
{
    Fraction result = Fraction(numerator * div.denominator, denominator * div.numerator);
    result.reduce();
    return result;
}

void Fraction::operator/=(const Fraction& div) 
{
    *this = *this / div;
    this->reduce();
}

int Fraction::operator==(const Fraction& second) {return (this->numerator == second.numerator && this->denominator == second.denominator);}
int Fraction::operator!=(const Fraction& second) {return (!(*this == second));}
int Fraction::operator>(const Fraction& second) {return ( this->numerator * second.denominator > second.numerator * this->denominator);}
int Fraction::operator<(const Fraction& second) {return ( this->numerator * second.denominator < second.numerator * this->denominator);}
int Fraction::operator>=(const Fraction& second) {return ( this->numerator * second.denominator >= second.numerator * this->denominator);}
int Fraction::operator<=(const Fraction& second) {return ( this->numerator * second.denominator <= second.numerator * this->denominator);}

Fraction::Fraction(int number) : Fraction(number, 1) {};

int Fraction::get_denominator() const {return denominator;};
int Fraction::get_numerator() const {return numerator;};
