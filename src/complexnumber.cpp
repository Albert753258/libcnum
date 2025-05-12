#include "../include/libcomplexnumber.h"
#include <cmath>
#include <iostream>


namespace libcnum {
    ComplexNumber::ComplexNumber(std::string num) {
        pInPower = false;

        //Числитель/знаменатель коэффициэнта/степени
        long coefficientNumerator;
        long coefficientDenominator;
        long powerNumerator;
        long powerDenominator;

        bool coefficientFound = false,
            powerFound = false;


        std::string tmp;
        int curPos = 0;
        char c;

        //Буферное число
        long long buf = 0;
        //TODO проверить хватит ли типа char для хранения количества знаков после запятой
        int afterDotCount = 0;
        bool dotFound = false,
            isMinus = false;

#pragma region Считываем коэффициэнт
        //1 часть парсинга - вытаскиваем коэффициэнт комплексного числа (идем до e)
        for(; curPos < num.length(); curPos++) {
            c = num[curPos];
            //Идем по строке до того, как найдем e или что-то кроме чисел
            if(c == 'e') {
                if(dotFound) {
                    coefficientDenominator = static_cast<long>(std::pow(10, afterDotCount));
                    // ReSharper disable once CppLocalVariableMightNotBeInitialized
                    //Если dotFound, то либо coefficientNumerator инициализирован, либо выкинут эксепшен
                    coefficientNumerator = coefficientNumerator * coefficientDenominator + buf;
                }
                else {
                    if(afterDotCount == 0) {
                        throw std::invalid_argument("Invalid complex number format: не введен коэффициэнт");
                    }
                    coefficientNumerator = buf;
                    coefficientDenominator = 1;
                }
                coefficientFound = true;
                break;
            }
            else if(c == '.') {
                if(dotFound) {
                    throw std::invalid_argument("Invalid complex number format: 2 точки в коэффициэнте");
                }
                if(afterDotCount == 0) {
                    throw std::invalid_argument("Invalid complex number format: не введен коэффициэнт");
                }
                afterDotCount = 0;
                dotFound = true;
                coefficientNumerator = buf;
                buf = 0;
                //Чтоб в буфер не ушла точка
                continue;
            }
            else if(c == '-') {
                if(dotFound) {
                    throw std::invalid_argument("Invalid complex number format: минус после точки и до e");
                }
                if(isMinus) {
                    throw std::invalid_argument("Invalid complex number format: 2 минуса в коэффициэнте");
                }
                isMinus = true;
                //Чтоб в буфер не ушел минус
                continue;
            }
            //c - не число, минус или точка
            else if(c < '0' or c > '9') {
                throw std::invalid_argument("Invalid complex number format: лишние символы в коэффициэнте");
            }
            //TODO проверка переполнения как в коде gcc
            buf *= 10;
            buf += c - '0';
            afterDotCount++;
        }
        //Значит в числе нету e - ошибка
        if(!coefficientFound) throw std::invalid_argument("Invalid complex number format: нет e");
        curPos++;

        if(isMinus) coefficientNumerator *= -1;

#pragma endregion

#pragma region Проверка наличия ^ после e
        if(curPos < num.length()) {
            c = num[curPos];
            if(c != '^') {
                throw std::invalid_argument("Invalid complex number format: нет ^");
            }
        }
        else {
            throw std::invalid_argument("Invalid complex number format: нет ^ после e");
        }
        curPos++;
#pragma endregion

#pragma region Считываем коэффициэнт
        dotFound = false;
        isMinus = false;
        buf = 0;
        afterDotCount = 0;
        //2 часть парсинга - идем до i
        for(; curPos < num.length(); curPos++) {
            c = num[curPos];
            //Идем по строке до того, как найдем e или что-то кроме чисел
            //Если встретили точку - значит парсим десятичную дробь как обыкновенную
            if(c == 'i') {
                if(dotFound) {
                    powerDenominator = static_cast<long>(std::pow(10, afterDotCount));
                    // ReSharper disable once CppLocalVariableMightNotBeInitialized
                    //Если dotFound, то либо powerNumerator инициализирован, либо выкинут эксепшен
                    powerNumerator = powerNumerator * powerDenominator + buf;
                }
                else {
                    if(afterDotCount == 0) {
                        throw std::invalid_argument("Invalid complex number format: не введен коэффициэнт");
                    }
                    powerNumerator = buf;
                    powerDenominator = 1;
                }
                powerFound = true;
                break;
            }
            else if(c == '.') {
                if(dotFound) {
                    throw std::invalid_argument("Invalid complex number format: 2 точки в степене");
                }
                if(afterDotCount == 0) {
                    throw std::invalid_argument("Invalid complex number format: не введен коэффициэнт");
                }
                afterDotCount = 0;
                dotFound = true;
                powerNumerator = buf;
                buf = 0;
                //Чтоб в буфер не ушла точка
                continue;
            }
            else if(c == '-') {
                if(dotFound) {
                    throw std::invalid_argument("Invalid complex number format: минус после точки и до i");
                }
                if(isMinus) {
                    throw std::invalid_argument("Invalid complex number format: 2 минуса в степени");
                }
                isMinus = true;
                //Чтоб в буфер не ушел минус
                continue;
            }
            else if(c < '0' or c > '9') {
                throw std::invalid_argument("Invalid complex number format: проверьте степень числа");
            }
            //TODO проверка переполнения как в коде gcc
            buf *= 10;
            buf += c - '0';
            afterDotCount++;
        }
        //Значит в числе нету i - ошибка
        if(!powerFound) throw std::invalid_argument("Invalid complex number format: отсутствует i");

        if(isMinus) powerNumerator *= -1;
        curPos++;
#pragma endregion

#pragma region Проверяем наличие P
        //Проверяем есть ли P в степени. Проверяем является ли степень дробью
        for(; curPos < num.length(); curPos++) {
            c = num[curPos];
            if(c == 'P') {
                if(pInPower) throw std::invalid_argument("Invalid complex number format: в степени может быть только 1 P");
                pInPower = true;
            }
            else {
                throw std::invalid_argument("Invalid complex number format: некорректный символ после i");
            }
        }
#pragma endregion

        // ReSharper disable once CppLocalVariableMightNotBeInitialized
        coefficient = FractionNum(coefficientNumerator, coefficientDenominator);
        // ReSharper disable once CppLocalVariableMightNotBeInitialized
        power = FractionNum(powerNumerator, powerDenominator);
    }

    ComplexNumber::ComplexNumber(const FractionNum coefficient_, const FractionNum power_, const bool pInPower_) {
        coefficient = coefficient_;
        power = power_;
        pInPower = pInPower_;
    }

    ComplexNumber::ComplexNumber() {
        coefficient = FractionNum();
        power = FractionNum();
        pInPower = false;
    }
}