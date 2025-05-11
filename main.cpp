#include <iostream>

#include "libcomplexnumber/libcomplexnumber.h"


int main() {
    std::string str;

    while (true) {
        std::cin >> str;
        auto num = libcomplexnumber::ComplexNumber(str);
        int i = 0;
    }
    return 0;
}
