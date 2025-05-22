#include "../include/libcomplexnumber.h"
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <cmath>

namespace libcnum {
    void FractionNum::simplify(long& numerator_, long& denominator_) {
        const long gcdVal = std::gcd(numerator_, denominator_);
        numerator_ /= gcdVal;
        denominator_ /= gcdVal;
    }

#pragma region Конструкторы, фабричный метод
    FractionNum::FractionNum(): pInNumerator(false), numerator(1), denominator(1) { }

    FractionNum::FractionNum(long numerator_, long denominator_, const bool pInNumerator_) {
        if(numerator_ == 0) {
            numerator = 0;
            denominator = 1;
            pInNumerator = false;
            return;
        }
        simplify(numerator_, denominator_);
        numerator = numerator_;
        denominator = denominator_;
        pInNumerator = pInNumerator_;
    }

    FractionNum::FractionNum(double num): pInNumerator(false) {
        const auto bits = *reinterpret_cast<unsigned long long*>(&num);
        static_assert(sizeof(double) == 8, "This program works only on 64-bit architecture. You should rewrite FractionNum::FractionNum(double num) constructor");
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
            throw std::overflow_error("This number is too big to fit in FractionNum without loosing it's presticion");
        }
        if(fraction & 0x8000000000000000) {
            throw std::logic_error("Unknown error, Sign bit(63th) mustn't be 1");
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
#pragma endregion

    //Вычислить дробь в десятичный double
    FractionNum::operator double() const {
        auto numerator_ = static_cast<double>(numerator);
        if(pInNumerator) {
            numerator_ *= M_PI;
        }
        return numerator_ / static_cast<double>(denominator);
    }

    bool FractionNum::operator== (const FractionNum& other) const {
        return (numerator == other.numerator)
        && (denominator == other.denominator)
        && (pInNumerator == other.pInNumerator);
    }

    bool FractionNum::operator== (const long& other) const {
        if (denominator != 1 || pInNumerator) return false;
        return numerator == other;
    }

    bool FractionNum::operator!= (const long& other) const {
        if (denominator != 1 || pInNumerator) return true;
        return numerator != other;
    }

#pragma region Математические операторы

    FractionNum FractionNum::operator/ (const FractionNum& other) const {
        if(!pInNumerator && other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) / static_cast<double>(other));
        }
        return FractionNum(numerator * other.denominator, denominator * other.numerator, pInNumerator && !other.pInNumerator);
    }

    FractionNum FractionNum::operator* (const FractionNum& other) const {
        if(pInNumerator && other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) * static_cast<double>(other));
        }
        return FractionNum(numerator * other.numerator, denominator * other.denominator, pInNumerator || other.pInNumerator);
    }

    FractionNum FractionNum::operator* (const long& other) const {
        return FractionNum(numerator * other, denominator, pInNumerator);
    }

    FractionNum FractionNum::operator+ (const FractionNum& other) const {
        if(pInNumerator != other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) + static_cast<double>(other));
        }
        return FractionNum(
            numerator * other.denominator + other.numerator * denominator,
            denominator * other.denominator,
            pInNumerator);
    }

    FractionNum FractionNum::operator- (const FractionNum& other) const {
        if(pInNumerator != other.pInNumerator) {
            //Дальнейшие вычисления невозможны без потери точности
            return FractionNum(static_cast<double>(*this) - static_cast<double>(other));
        }
        return FractionNum(
            numerator * other.denominator - other.numerator * denominator,
            denominator * other.denominator,
            pInNumerator);
    }
#pragma endregion

    std::ostream& operator<<(std::ostream& os, const FractionNum& num) {
        os << num.ToString();
        return os;
    }

    std::string FractionNum::ToString() const{
        if(numerator == 0) {
            return "";
        }
        std::string ret;
        if(pInNumerator) {
            if(numerator != 1) {
                ret += std::to_string(numerator);
            }
            ret += "P";
        }
        else {
            ret += std::to_string(numerator);
        }
        if(denominator != 1) {
            ret += "/" + std::to_string(denominator);
        }
        return ret;
    }
}
