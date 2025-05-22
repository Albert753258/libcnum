#include "../include/libcomplexnumber.h"
#include <cmath>
#include <iostream>


namespace libcnum {
    //Обработать часть (степень или коэффициэнт) комплексного числа
    void parseSinglePart(const char breakSymbol, long& numerator, long& denominator, int& curPos, const std::string& num) {
        long long buf = 0;
        char afterDotCount = 0;
        bool dotFound = false,
            isMinus = false,
            isFraction = false;
        for(; curPos < num.length(); curPos++) {
            const char c = num[curPos];
            //Идем по строке до того, как найдем e или что-то кроме чисел
            if(c == breakSymbol) {
                if(isFraction) {
                    denominator = buf;
                }
                else{
                    if(dotFound) {
                        if(afterDotCount == 0)
                            throw std::invalid_argument("Invalid complex number format(" + num + "): after dot should be number, but found e");
                        denominator = static_cast<long>(std::pow(10, afterDotCount));
                        //Если dotFound, то либо numerator инициализирован, либо выкинут эксепшен
                        numerator = numerator * denominator + buf;
                    }
                    else {
                        if(afterDotCount == 0)
                            numerator = 1;
                        else
                            numerator = buf;
                        denominator = 1;
                    }
                }
                curPos++;
                if(isMinus)
                    numerator *= -1;
                return;
            }
            else if(c == '.') {
                if(dotFound)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): 2 dots in one number");
                if(afterDotCount == 0)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): number cannot start with dot");
                if(isFraction)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): fraction cannot contain dot");

                afterDotCount = 0;
                dotFound = true;
                numerator = buf;
                buf = 0;
                //Чтоб в буфер не ушла точка
                continue;
            }
            else if(c == '-') {
                if(dotFound)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): minus cannot be after dot");
                if(isMinus)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): 2 minuses in one number");
                if(isFraction)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): minus cannot be after /");

                isMinus = true;
                //Чтоб в буфер не ушел минус
                continue;
            }
            else if(c == '/') {
                if(dotFound)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): fraction cannot contain dot");
                if(isFraction)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): 2 / in one number");
                isFraction = true;
                numerator = buf;
                buf = 0;
                continue;
            }
            else if(c < '0' or c > '9')
                throw std::invalid_argument("Invalid complex number format(" + num + "): invalid symbol " + c);

            buf *= 10;
            buf += c - '0';
            afterDotCount++;
        }
        throw std::invalid_argument("Invalid complex number format(" + num + "): missing " + breakSymbol);
    }

    ComplexNumber::ComplexNumber(const std::string &num) {
        bool pInPower = false;

        //Числитель/знаменатель коэффициэнта/степени
        long coefficientNumerator = 0,
            coefficientDenominator = 0,
            powerNumerator = 0,
            powerDenominator = 0;

        int curPos = 0;
        char c;
        //1 часть парсинга - вытаскиваем коэффициэнт комплексного числа (идем до e)
        parseSinglePart('e', coefficientNumerator, coefficientDenominator, curPos, num);

#pragma region Проверка наличия ^ после e
        if(curPos < num.length()) {
            c = num[curPos];
            if(c != '^') {
                throw std::invalid_argument("Invalid complex number format(" + num + "): ^ should be after e, found " + c);
            }
        }
        else {
            throw std::invalid_argument("Invalid complex number format(" + num + "): ^ should be after e");
        }
        curPos++;
#pragma endregion

        //2 часть парсинга - вытаскиваем степень комплексного числа (идем до i)
        parseSinglePart('i', powerNumerator, powerDenominator, curPos, num);


#pragma region Проверяем наличие P после i
        //Проверяем есть ли P в степени. Проверяем является ли степень дробью
        for(; curPos < num.length(); curPos++) {
            c = num[curPos];
            if(c == 'P') {
                if(pInPower)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): after P should be nothing, found" + c);
                pInPower = true;
            }
            else {
                if(pInPower)
                    throw std::invalid_argument("Invalid complex number format(" + num + "): invalid symbol after P: " + c);
                throw std::invalid_argument("Invalid complex number format(" + num + "): invalid symbol after i: " + c);
            }
        }
#pragma endregion

        coefficient = FractionNum(coefficientNumerator, coefficientDenominator, false);
        power = FractionNum(powerNumerator, powerDenominator, pInPower);
    }

    ComplexNumber::ComplexNumber(const FractionNum &coefficient_, const FractionNum &power_) {
        coefficient = coefficient_;
        power = power_;
    }

    ComplexNumber::ComplexNumber() {
        coefficient = FractionNum();
        power = FractionNum();
    }

    ComplexNumber ComplexNumber::operator/(const ComplexNumber &other) const {
        const auto newCoef = coefficient / other.coefficient;
        const auto newPower = power - other.power;
        return ComplexNumber(newCoef, newPower);
    }

    ComplexNumber ComplexNumber::operator*(const ComplexNumber &other) const {
        const auto newCoef = coefficient * other.coefficient;
        const auto newPower = power + other.power;
        return ComplexNumber(newCoef, newPower);

    }

    bool ComplexNumber::operator==(const ComplexNumber &other) const {
        return coefficient == other.coefficient && power == other.power;
    }
    double roundTo3Dec(double var)
    {
        double value = static_cast<float>((int)(var * 100 + 0.5));
        return value / 100;
    }

    ComplexNumber InitFromAlg(const double a, const double b) {
        const double coefficient = roundTo3Dec(sqrt(a * a + b * b)),
            power = roundTo3Dec(atan2(b, a));
        return ComplexNumber(FractionNum(coefficient), FractionNum(power));
    }

    ComplexNumber ComplexNumber::operator+(const ComplexNumber &other) const {
        if(power == other.power) {
            return ComplexNumber(coefficient + other.coefficient, power);
        }
        if(power == (other.power * -1)) {
            return ComplexNumber(coefficient - other.coefficient, power);
        }
        const auto coefficient1 = static_cast<double>(coefficient),
            coefficient2 = static_cast<double>(other.coefficient),
            power1 = static_cast<double>(power),
            power2 = static_cast<double>(other.power);

        const double a1 = coefficient1 * cos(power1),
            a2 = coefficient2 * cos(power2),
            b1 = coefficient1 * sin(power1),
            b2 = coefficient2 * sin(power2);

        return InitFromAlg(a1 + a2, b1 + b2);
    }

    ComplexNumber ComplexNumber::operator-(const ComplexNumber &other) const {
        if(power == other.power) {
            return ComplexNumber(coefficient - other.coefficient, power);
        }
        const auto coefficient1 = static_cast<double>(coefficient),
            coefficient2 = static_cast<double>(other.coefficient),
            power1 = static_cast<double>(power),
            power2 = static_cast<double>(other.power);

        const double a1 = coefficient1 * cos(power1),
            a2 = coefficient2 * cos(power2),
            b1 = coefficient1 * sin(power1),
            b2 = coefficient2 * sin(power2);

        return InitFromAlg(a1 - a2, b1 - b2);
    }

    ComplexNumber ComplexNumber::pow(long power) const {
        ComplexNumber ret = *this;
        if(power <= 0) {
            throw std::logic_error("Not implemented");
        }
        while(power > 1) {
            power--;
            ret = ret * *this;
        }
        return ret;
    }

    std::ostream& operator<<(std::ostream& os, const ComplexNumber& num) {
        if(num.coefficient == 0) {
            os << "0";
            return os;
        }

        if(!(num.coefficient == 1)) {
            os << num.coefficient;
        }
        os << "e^";
        if(num.power.numerator != 1)
            os << num.power.numerator;
        os << "i";
        if(num.power.pInNumerator) {
            os << "P";
        }
        if(num.power.denominator != 1) {
            os << "/" << num.power.denominator;
        }

        return os;
    }
}