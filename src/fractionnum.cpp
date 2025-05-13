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

    FractionNum::FractionNum(double num): pInNumerator(false) {
        const auto bits = *reinterpret_cast<unsigned long long*>(&num);
        static_assert(sizeof(double) == 8, "Для запуска не на 64-битной системе адаптируйте метод под вашу архитектуру процессора");
        //11 бит, влезут в int
        int exponent = static_cast<int>((bits >> 52 & 0x7FF) - 1023);
        //Экспонента 0 - спецслучай для 0
        if(exponent == -1023 ) {
            numerator = 0;
            denominator = 1;
            return;
        }
        //Отбрасываем 63-52 бит, в 52 бит подпихиваем 1 (нехранимая мнимая единица)
        unsigned long long fraction = (bits & 0x0FFFFFFFFFFFFF) | 0x10000000000000;

        //Из-за мнимой единицы, числитель как бы умножается на 2^52, так что и знаменатель надо такой
        denominator = 1LL << 52;

        if (exponent > 0) {
            //Делим знаменатель на 2 пока это возможно
            while((exponent != 0) && !(denominator & 0x1)) {
                exponent--;
                denominator = denominator >> 1;
            }
            //Умножаем числитель на 2 пока это возможно
            while((exponent != 0) && !(fraction & 0x3FFFFFFFFFFFFFFF)) {
                exponent--;
                fraction = fraction << 1;
            }
        }
        else {
            //Делим числитель на 2 пока это возможно
            while((exponent != 0) && !(fraction & 0x1)) {
                exponent++;
                fraction = fraction >> 1;
            }
            //Умножаем знаменатель на 2 пока это возможно
            while((exponent != 0) && !(denominator & 0x3FFFFFFFFFFFFFFF)) {
                exponent++;
                denominator = denominator << 1;
            }
        }
        if(exponent != 0) {
            throw std::overflow_error("Это число не может быть представлено в виде обыкновенной дроби без потери точности");
        }
        if(fraction & 0x8000000000000000) {
            throw std::logic_error("Знаковый бит(63) не может быть равен 1. Ошибка в программе, не в числе");
        }
        numerator = static_cast<long>(fraction);

        if(bits >> 63) numerator *= -1;

        simplify(numerator, denominator);
    }

    FractionNum CreateFractionNum(long numerator_, long denominator_, const bool pInNumerator_) {
        if(denominator_ == 0) {
            throw std::invalid_argument("Denominator musn't be 0");
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
        bool pInNumerator_ = false;
        if(pInNumerator) {
            if(!other.pInNumerator) {
                pInNumerator_ = true;
            }
        }
        else if(other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) / static_cast<double>(other));
        }

        const long new_num = numerator * other.denominator;
        const long new_denom = denominator * other.numerator;
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
