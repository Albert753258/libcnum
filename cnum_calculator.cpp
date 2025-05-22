#include <iostream>

#include "include/libcomplexnumber.h"


const std::string helpMessage = R"(USAGE: cnum_calculator [OPTION] "<complex number expression>"
Currently this program can make only one operation for one time
Options:
  -f=format     set output format to format. Valid formats are algebraic, triganometric, exponential
  -a            equivalent to -f=algebraic
  -t            equivalent to -f=triganometric
  -e            default, equivalent to -f=exponential
  -h, --help    show this message

Complex number expressions should be in folowing format:
<complex number 1> <operator> <complex number 1> - calculate expression with to complex numbers.
    Space required before and after opertor.
    Supported operators are +, -, *, /
(<complex number>)^<power> - pow complex number to power. Power can be any natural number greater than 0

Complex numbers should be in folowing format:
<coefficient>e^<power>i<P>
    Both coefficient and power can be integer or decimal/common fraction
    coefficient, power and P aren't required

Ecamples of valid expressions:
2e^1/4iP + 3e^1/4iP
e^0.5iP + e^-0.5iP
4e^1/3iP - 2e^1/3iP
2e^1/6iP * 3e^1/3iP
6e^5/6iP / 2e^1/3iP
(2e^0.25iP)^2
    )";

int main(const int argc, char *argv[]) {
    if(argc == 1) {
        std::cout << helpMessage << std::endl;
        return -1;
    }

    int format = -1;
    for (int i = 1; i < argc; i++) {
        auto arg = std::string(argv[i]);
        if(arg == "--help" || arg == "-h") {
            std::cout << helpMessage << std::endl;
            return 0;
        }
        if(arg == "-a" || arg == "-f=algebraic") {
            format = 1;
            break;
        }
        if(arg == "-t" || arg == "-f=triganometric") {
            format = 2;
            break;
        }
        if(arg == "-e" || arg == "-f=exponential") {
            format = 3;
            break;
        }
    }
    int curPos;
    if(format == -1) {
        format = 3;
        curPos = 1;
    }
    else curPos = 2;

    try {
        const auto res = libcnum::calculateExpression(argv[curPos]);
        if(format == 1) {
            std::cout << res.ToAlgebraic() << std::endl;
        }
        else if(format == 2) {
            std::cout << res.ToTriganometric() << std::endl;
        }
        else {
            std::cout << res << std::endl;
        }
        return 0;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}