#include <iostream>

#include "include/libcomplexnumber.h"


const std::string helpMessage = R"(Usage: cnum_calculator [OPTION]... "EXPRESSION"
Perform a single operation on complex numbers and print result in specified format.

Options:
  -a, --algebraic       display result in algebraic form (a + bi)
  -t, --trigonometric   display result in trigonometric form (r(cosθ + i·sinθ))
  -e, --exponential     display result in exponential form (re^θiP, default)
  -h, --help            display this help and exit
  --version             output version information and exit

EXPRESSION format:
  <num1> <op> <num2>    Perform single operation (+, -, *, /)
  (<num>)^<power>       Raise complex number to a natural power

  Notes:
  - Spaces around <op> are mandatory.
  - <power> must be a positive integer.

COMPLEX NUMBER format:
  [<coeff>]e^[<power>]i[P]

  Where:
  - <coeff>             real coefficient (integer/decimal/fraction)
  - <power>             exponent angle (integer/decimal/fraction)
  - P                   'pi' multiplier
  All components are optional

Examples:
  cnum_calculator -a "2e^1/4iP + 3e^1/4iP"
  cnum_calculator -t "4e^1/3iP - 2e^1/3iP"
  cnum_calculator -e "6e^5/6iP / 2e^1/3iP"
  cnum_calculator --algebraic "e^0.5iP + e^-0.5iP"
  cnum_calculator --trigonometric "2e^1/6iP * 3e^1/3iP"
  cnum_calculator --exponential "(2e^0.25iP)^2"
    )";
const std::string versionMessage = R"(cnum_calculator 0.1
Made by Albert753258
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
        if(arg == "--version") {
            std::cout << versionMessage << std::endl;
            return 0;
        }
        if(arg == "-a" || arg == "--algebraic") {
            format = 1;
            break;
        }
        if(arg == "-t" || arg == "--trigonometric") {
            format = 2;
            break;
        }
        if(arg == "-e" || arg == "--exponential") {
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