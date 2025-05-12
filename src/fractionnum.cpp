#include "../include/libcomplexnumber.h"
#include <numeric>
#include <stdexcept>
#include <iostream>

namespace libcnum {
    void FractionNum::simplify(long& numerator_, long& denominator_) {
        const long gcd_val = std::gcd(numerator_, denominator_);
        numerator_ /= gcd_val;
        denominator_ /= gcd_val;
    }

    FractionNum::FractionNum(): numerator(1), denominator(1) { }

    FractionNum::FractionNum(long numerator_, long denominator_) {
        simplify(numerator_, denominator_);
        numerator = numerator_;
        denominator = denominator_;
    }

    FractionNum CreateFractionNum(long numerator_, long denominator_) {
        if(denominator_ == 0) {
            throw std::invalid_argument("denominator musn't be 0");
        }
        if(denominator_ < 0) {
            numerator_ *= -1;
            denominator_ *= -1;
        }
        return FractionNum(numerator_, denominator_);
    }

    //Вычислить дробь в десятичный double
    FractionNum::operator double() const {
        if(denominator == 1) {
            return numerator;
        }
        return static_cast<double>(numerator) / denominator;
    }

    FractionNum FractionNum::operator/ (const FractionNum& other) const {
        const long new_num = numerator * other.denominator;
        const long new_denom = denominator * other.numerator;
        return FractionNum(new_num, new_denom);
    }

    FractionNum FractionNum::operator* (const FractionNum& other) const {
        const long new_num = numerator * other.numerator;
        const long new_denom = denominator * other.denominator;
        return FractionNum(new_num, new_denom);
    }

    bool FractionNum::operator== (const FractionNum& other) const {
        return (numerator == other.numerator) && (denominator == other.denominator);
    }

    FractionNum FractionNum::operator+ (const FractionNum& other) const {
        const long new_num = numerator * other.denominator + other.numerator * denominator;
        const long new_denom = denominator * other.denominator;
        return FractionNum(new_num, new_denom);
    }

    FractionNum FractionNum::operator- (const FractionNum& other) const {
        const long new_num = numerator * other.denominator - other.numerator * denominator;
        const long new_denom = denominator * other.denominator;
        return FractionNum(new_num, new_denom);
    }

    std::ostream& operator<<(std::ostream& os, const FractionNum& num) {
        os << num.numerator << '/' << num.denominator;
        return os;
    }
}
