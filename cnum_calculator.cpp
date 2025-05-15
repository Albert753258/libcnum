#include <iostream>

#include "include/libcomplexnumber.h"

int main(int argc, char *argv[]) {
    std::string str;

    while (true) {
        std::cout << "Enter expression: ";
        std::getline(std::cin, str);
        try {
            std::cout << libcnum::parseExpression(str) << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}