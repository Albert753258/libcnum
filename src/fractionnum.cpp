#include "../include/libcomplexnumber.h"
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <cmath>

namespace libcnum {
    void FractionNum::simplify(long& numerator_, long& denominator_) {
        const long gcd_val = std::gcd(numerator_, denominator_);
        numerator_ /= gcd_val;
        denominator_ /= gcd_val;
    }

    FractionNum::FractionNum(): pInNumerator(false), numerator(1), denominator(1) { }

    FractionNum::FractionNum(long numerator_, long denominator_, bool pInNumerator_) {
        simplify(numerator_, denominator_);
        numerator = numerator_;
        denominator = denominator_;
        pInNumerator = pInNumerator_;
    }

    // Преобразуем double в дробь (числитель / знаменатель)
    std::pair<int64_t, int64_t> double_to_exact_fraction(double x) {
        unsigned long long bits = *reinterpret_cast<unsigned long long*>(&x);
        int sign = (bits >> 63) ? -1 : 1;
        int exponent = ((bits >> 52) & 0x7FF) - 1023;
        unsigned long long mantissa = (bits & 0x0FFFFFFFFFFFFF) | (exponent == -1023 ? 0 : 0x10000000000000);

        // Числитель = мантисса * 2^max(0, exponent)
        // Знаменатель = 2^(52 - min(0, exponent))
        auto numerator = static_cast<int64_t>(mantissa);
        int64_t denominator = 1LL << 52;

        if (exponent > 0) {
            numerator *= (1LL << exponent);
        } else {
            denominator *= (1LL << -exponent);
        }

        // Упрощаем дробь
        int64_t gcd = std::gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;

        return {sign * numerator, denominator};
    }

    //TODO досканально протестить этот код, разобраться как оно вообще работает
    FractionNum::FractionNum(double num) {
        unsigned long long bits = *reinterpret_cast<unsigned long long*>(&num);
        int exponent = ((bits >> 52) & 0x7FF) - 1023;
        unsigned long long mantissa = (bits & 0x0FFFFFFFFFFFFF) | (exponent == -1023 ? 0 : 0x10000000000000);

        // Числитель = мантисса * 2^max(0, exponent)
        // Знаменатель = 2^(52 - min(0, exponent))
        numerator = static_cast<int64_t>(mantissa);
        denominator = 1LL << 52;

        if (exponent > 0) {
            numerator *= (1LL << exponent);
        } else {
            denominator *= (1LL << -exponent);
        }
        if(bits >> 63) numerator *= -1;

        simplify(numerator, denominator);
        pInNumerator = false;
    }

    FractionNum CreateFractionNum(long numerator_, long denominator_, const bool pInNumerator_) {
        if(denominator_ == 0) {
            throw std::invalid_argument("denominator musn't be 0");
        }
        if(denominator_ < 0) {
            numerator_ *= -1;
            denominator_ *= -1;
        }
        return FractionNum(numerator_, denominator_, pInNumerator_);
    }

    //Вычислить дробь в десятичный double
    FractionNum::operator double() const {
        auto numerator_ = static_cast<double>(numerator);
        if(pInNumerator) {
            numerator_ *= M_PI;
        }
        return numerator_ / static_cast<double>(denominator);
    }

    FractionNum FractionNum::operator/ (const FractionNum& other) const {
        const long new_num = numerator * other.denominator;
        const long new_denom = denominator * other.numerator;
        bool pInNumerator_;
        if(pInNumerator) {
            if(other.pInNumerator) {
                pInNumerator_ = false;
            }
            else {
                pInNumerator_ = true;
            }
        }
        else if(other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) / static_cast<double>(other));
        }
        else pInNumerator_ = false;

        return FractionNum(new_num, new_denom, pInNumerator_);
    }

    FractionNum FractionNum::operator* (const FractionNum& other) const {
        const long new_num = numerator * other.numerator;
        const long new_denom = denominator * other.denominator;
        if(pInNumerator && other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) * static_cast<double>(other));
        }
        return FractionNum(new_num, new_denom, pInNumerator || other.pInNumerator);
    }

    bool FractionNum::operator== (const FractionNum& other) const {
        return (numerator == other.numerator) && (denominator == other.denominator) && (pInNumerator == other.pInNumerator);
    }

    FractionNum FractionNum::operator+ (const FractionNum& other) const {
        const long new_num = numerator * other.denominator + other.numerator * denominator;
        const long new_denom = denominator * other.denominator;
        if(pInNumerator != other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) + static_cast<double>(other));
        }
        return FractionNum(new_num, new_denom, pInNumerator);
    }

    FractionNum FractionNum::operator- (const FractionNum& other) const {
        const long new_num = numerator * other.denominator - other.numerator * denominator;
        const long new_denom = denominator * other.denominator;
        if(pInNumerator != other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) - static_cast<double>(other));
        }
        return FractionNum(new_num, new_denom, pInNumerator);
    }

    std::ostream& operator<<(std::ostream& os, const FractionNum& num) {
        os << num.numerator;
        if(num.pInNumerator) {
            os << "P";
        }
        if(num.denominator != 1) {
            os << '/' << num.denominator;
        }
        return os;
    }
}
