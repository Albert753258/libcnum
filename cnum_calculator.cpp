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
        auto res = libcnum::calculateExpression(argv[curPos]);
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