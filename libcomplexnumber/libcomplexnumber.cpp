//
// Created by albert on 5/10/25.
//

#include "libcomplexnumber.h"

#include <cmath>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <cstring>

namespace libcomplexnumber {
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

    FractionNum FractionNum::CreateFractionNum(long numerator_, long denominator_) {
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

    ComplexNumber::ComplexNumber(std::string num) {
        double coefficient_ = 0;
        bool coefficientFound = false,
            powerIsDouble = false,
            iFound = false,
            //Является ли степень обыкновененой дробью
            powerIsRational = false;

        //Десятичные дроби считаем как обыкновенные.
        //Целая часть дроби
        long powerBeforeDot;
        //Количество знаков после запятой
        long powerAfterDotCount = 0;

        long powerDenominator;
        long powerNumerator;

        std::string tmp;
        int curPos = 0;
        char c;
        //1 часть парсинга - вытаскиваем коэффициэнт комплексного числа (идем до e)
        for(; curPos < num.length(); curPos++) {
            c = num[curPos];
            //Идем по строке до того, как найдем e или что-то кроме чисел
            if(c == 'e') {
                coefficient_ = std::stod(tmp);
                coefficientFound = true;
                break;
            }
            //c - не число, минус или точка
            else if((c < '0' or c > '9') && !(c == '.' || c == '-')) {
                throw std::invalid_argument("Invalid complex number format!");
            }
            tmp += c;
        }
        curPos++;
        tmp.clear();

        //Значит в числе нету e - ошибка
        if(!coefficientFound) throw std::invalid_argument("Invalid complex number format: нет e");

        if(curPos < num.length()) {
            c = num[curPos];
            if(c != '^') {
                throw std::invalid_argument("Invalid complex number format: нет ^");
            }
        }
        else {
            throw std::invalid_argument("Invalid complex number format: нет ^");
        }
        curPos++;

        //2 часть парсинга - идем до i. Если встретили точку - значит степень указана как обыкновенная дробь
        for(; curPos < num.length(); curPos++) {
            c = num[curPos];
            powerAfterDotCount++;
            //Идем по строке до того, как найдем e или что-то кроме чисел
            //Если встретили точку - значит парсим десятичную дробь как обыкновенную
            if(c == '.') {
                if(powerIsDouble) {
                    //в степени 2 точки
                    throw std::invalid_argument("Invalid complex number format!");
                }
                powerBeforeDot = std::stol(tmp);
                powerAfterDotCount = 0;
                powerIsDouble = true;
                tmp.clear();
            }
            else if(c == 'i') {
                //Если степень введена в виде десятичной дроби
                if(powerIsDouble) {
                    tmp = tmp.substr(1);
                    //То, что после запятой в обыкновенной дроби
                    const long powerAfterDot = std::stol(tmp);
                    powerDenominator = static_cast<long>(std::pow(10, powerAfterDotCount));
                    //powerBeforeDot не может быть неинициализированной
                    powerNumerator = powerBeforeDot * powerDenominator + powerAfterDot;

                }
                //Иначе сразу как long
                else {
                    powerNumerator = std::stol(tmp);
                    powerDenominator = 1;
                }
                iFound = true;
                break;
            }
            else if(c < '0' or c > '9') {
                throw std::invalid_argument("Invalid complex number format: проверьте степень числа");
            }
            tmp += c;
        }
        //Значит в числе нету i - ошибка
        if(!iFound) throw std::invalid_argument("Invalid complex number format");

        curPos++;
        tmp.clear();

        //Проверяем есть ли P в степени. Проверяем является ли степень дробью
        for(; curPos < num.length(); curPos++) {
            c = num[curPos];
            //4. Ищем P
            if(c == 'P') {
                if(pInPower) {
                    //Несколько P
                    throw std::invalid_argument("Invalid complex number format!");
                }
                pInPower = true;
            }
            else if(c == '/') {
                if(powerIsDouble) {
                    //В числителе дроби не может быть десятичной дроби
                    throw std::invalid_argument("Invalid complex number format!");
                }
                powerIsRational = true;
                break;
            }
            else {
                //После i должен быть либо P, знаменатель дроби
                throw std::invalid_argument("Invalid complex number format!");
            }
        }
        curPos++;

        //Если дробь обыкновенная - считываем знаменатель
        if(powerIsRational) {
            for(; curPos < num.length(); curPos++) {
                c = num[curPos];
                if(c < '0' or c > '9') {
                    //В знаменателе шляпа какая-то
                    throw std::invalid_argument("Invalid complex number format!");
                }
                tmp += c;
            }
            //Нет знаменателя
            if(tmp.empty()) {
                throw std::invalid_argument("Invalid complex number format!");
            }
            powerDenominator = std::stol(tmp);
        }
        else if(curPos < num.length()){
            //Лишние символы после i или P
            throw std::invalid_argument("Invalid complex number format!");
        }
        //Число введено корректно, в степени обыкновенная дробь
        else {
            power = FractionNum(powerNumerator, powerDenominator);
        }
        coefficient = coefficient_;
        power = FractionNum(powerNumerator, powerDenominator);
    }

    ComplexNumber::ComplexNumber(const double coefficient_, const FractionNum power_, const bool pInPower_) {
        coefficient = coefficient_;
        power = power_;
        pInPower = pInPower_;
    }
}
