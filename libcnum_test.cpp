#include <stdexcept>

#include "include/libcomplexnumber.h"

void runComplexNumberParsingTestPositive(const std::string &num, const long cNum, const long cDen, const long pNum, const long pDen, const bool pPow) {
    bool flag = false;
    try {
        flag = !libcnum::ComplexNumber(num).assert_test(cNum, cDen, pNum, pDen, pPow);
    }
    catch (...){
        throw std::logic_error("Не спарсилась строка спарсилась строка " + num);
    }
    if(flag) {
        throw std::logic_error("некорректно спарсилась строка " + num);
    }
}

void runComplexNumbeTestNegative(const std::string &num) {
    try {
        auto i = libcnum::ComplexNumber(num);
    }
    catch (...){
        return;
    }
    throw std::logic_error("Спарсилась строка " + num + ", которая не должна парситься");
}

int main() {
#pragma region тесты на парсинг корректных чисел

    runComplexNumberParsingTestPositive("1/2e^1/2i", 1, 2, 1, 2, false);
    runComplexNumberParsingTestPositive("1/2e^1/2iP", 1, 2, 1, 2, true);
    runComplexNumberParsingTestPositive("1/2e^0.5iP", 1, 2, 1, 2, true);
    runComplexNumberParsingTestPositive("11e^44i", 11, 1, 44, 1, false);
    runComplexNumberParsingTestPositive("11e^44iP", 11, 1, 44, 1, true);
    runComplexNumberParsingTestPositive("11e^44.5i", 11, 1, 445, 10, false);
    runComplexNumberParsingTestPositive("11e^44.5iP", 11, 1, 445, 10, true);

    runComplexNumberParsingTestPositive("11.1e^44i", 111, 10, 44, 1, false);
    runComplexNumberParsingTestPositive("11.1e^44iP", 111, 10, 44, 1, true);
    runComplexNumberParsingTestPositive("11.1e^44.5i", 111, 10, 445, 10, false);
    runComplexNumberParsingTestPositive("11.1e^44.5iP", 111, 10, 445, 10, true);

    runComplexNumberParsingTestPositive("11.12e^44i", 1112, 100, 44, 1, false);
    runComplexNumberParsingTestPositive("11.12e^44iP", 1112, 100, 44, 1, true);
    runComplexNumberParsingTestPositive("11.12e^44.5i", 1112, 100, 445, 10, false);
    runComplexNumberParsingTestPositive("11.12e^44.5iP", 1112, 100, 445, 10, true);
    runComplexNumberParsingTestPositive("e^44.5iP", 1, 1, 445, 10, true);
    runComplexNumberParsingTestPositive("-e^44.5iP", -1, 1, 445, 10, true);
    runComplexNumberParsingTestPositive("e^iP", 1, 1, 1, 1, true);
    runComplexNumberParsingTestPositive("e^-iP", 1, 1, -1, 1, true);
#pragma endregion

#pragma region тесты на парсинг некорректных чисел
    runComplexNumbeTestNegative("1/2e^1/2Pi");
    runComplexNumbeTestNegative("1/2.5e^1/2Pi");
    runComplexNumbeTestNegative("1/2e^1/2");
    runComplexNumbeTestNegative("1/2e^i1/2");
    runComplexNumbeTestNegative("/2e^i1/2");
    runComplexNumbeTestNegative("1/0e^i1/2");
    runComplexNumbeTestNegative("1/e^1/2");
    runComplexNumbeTestNegative("/e^1/2");
    runComplexNumbeTestNegative("11e^44P");
    runComplexNumbeTestNegative("11e^44.5");
    runComplexNumbeTestNegative("11e^44.5P");
    runComplexNumbeTestNegative("11e^44.5Pi");
    runComplexNumbeTestNegative("11.1e^44");
    runComplexNumbeTestNegative("11.1e^44P");
    runComplexNumbeTestNegative("11.1e^44.5");
    runComplexNumbeTestNegative("11.1e^44.5P");
    runComplexNumbeTestNegative("11.1e^44.5Pi");
    runComplexNumbeTestNegative("11^44");
    runComplexNumbeTestNegative("11^44P");
    runComplexNumbeTestNegative("11^44.5");
    runComplexNumbeTestNegative("11^44.5P");
    runComplexNumbeTestNegative("11^44.5Pi");
    runComplexNumbeTestNegative("11.1^44");
    runComplexNumbeTestNegative("11.1^44P");
    runComplexNumbeTestNegative("11.1^44.5");
    runComplexNumbeTestNegative("11.1^44.5P");
    runComplexNumbeTestNegative("11.1^44.5Pi");
    runComplexNumbeTestNegative("11e44");
    runComplexNumbeTestNegative("11e44P");
    runComplexNumbeTestNegative("11e44.5");
    runComplexNumbeTestNegative("11e44.5P");
    runComplexNumbeTestNegative("11e44.5Pi");
    runComplexNumbeTestNegative("11.1e44");
    runComplexNumbeTestNegative("11.1e44P");
    runComplexNumbeTestNegative("11.1e44.5");
    runComplexNumbeTestNegative("11.1e44.5P");
    runComplexNumbeTestNegative("11.1e44.5Pi");
    runComplexNumbeTestNegative("e^44");
    runComplexNumbeTestNegative("e^44P");
    runComplexNumbeTestNegative("e^44.5");
    runComplexNumbeTestNegative("e^44.5P");
    runComplexNumbeTestNegative("e^44.5Pi");
    runComplexNumbeTestNegative(".1e^44");
    runComplexNumbeTestNegative(".1e^44P");
    runComplexNumbeTestNegative(".1e^44.5");
    runComplexNumbeTestNegative(".1e^44.5P");
    runComplexNumbeTestNegative(".1e^44.5Pi");
    runComplexNumbeTestNegative("11e^");
    runComplexNumbeTestNegative("11e^P");
    runComplexNumbeTestNegative("11e^.5");
    runComplexNumbeTestNegative("11e^.5P");
    runComplexNumbeTestNegative("11e^.5Pi");
    runComplexNumbeTestNegative("11.1e^");
    runComplexNumbeTestNegative("11.1e^P");
    runComplexNumbeTestNegative("11.1e^.5");
    runComplexNumbeTestNegative("11.1e^.5P");
    runComplexNumbeTestNegative("11.1e^.5Pi");
    runComplexNumbeTestNegative("11e^44.5.2");
    runComplexNumbeTestNegative("11e^44.5P.");
    runComplexNumbeTestNegative("11e^44.5Pi.");
    runComplexNumbeTestNegative("11.1e^44.");
    runComplexNumbeTestNegative("11.1e^44P.");
    runComplexNumbeTestNegative("11.1e^44.5.");
    runComplexNumbeTestNegative("11.1e^44.5P.");
    runComplexNumbeTestNegative("11.1e^44.5Pi.");
    runComplexNumbeTestNegative("11");
    runComplexNumbeTestNegative("11.1");
    runComplexNumbeTestNegative("sdfwef");
#pragma endregion
    return 0;
}