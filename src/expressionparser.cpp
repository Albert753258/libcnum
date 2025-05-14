#include "../include/libcomplexnumber.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>


namespace libcnum {
    ComplexNumber parseExpression(const std::string &expr) {
        if(expr[0] == '(') {
            int curPos = 1;
            std::string num1;
            bool num1Found = false;
            for(; curPos < expr.size(); curPos++) {
                const char c = expr[curPos];
                if(c == ')') {
                    num1Found = true;
                    break;
                }
                num1 += c;
            }
            if(!num1Found)
                throw std::invalid_argument("Unable to split expression");
            ComplexNumber n1;
            try {
                n1 = ComplexNumber(num1);
            }
            catch (...) {
                //TODO ошибка глушится
                throw std::invalid_argument("Unable to parse first number");
            }
            curPos++;
            if(curPos < expr.size()) {
                const char c = expr[curPos];
                if(c != '^') {
                    throw std::invalid_argument("Invalid symbol (should be ^): " + c);
                }
            }
            else
                throw std::invalid_argument("Missing ^");
            curPos++;



            bool isMinus = false;
            long buf = 0;
            for(; curPos < expr.length(); curPos++) {
                const char c = expr[curPos];
                if(c == '-') {
                    if(isMinus)
                        throw std::invalid_argument("Invalid expression(" + expr + "): 2 minuses in power");

                    isMinus = true;
                    //Чтоб в буфер не ушел минус
                    continue;
                }
                else if(c < '0' or c > '9')
                    throw std::invalid_argument("Invalid expression(" + expr + "): invalid symbol " + c);

                buf *= 10;
                buf += c - '0';
            }

            return n1.pow(buf);

        }
        else {
            std::string num1,
                num2;
            bool num1Found = false;
            int curPos = 0;
            char oper;
            for(; curPos < expr.size(); curPos++) {
                const char c = expr[curPos];
                if(c == ' ') {
                    num1Found = true;
                    break;
                }
                num1 += c;
            }
            if(!num1Found)
                throw std::invalid_argument("Unable to split expression");
            curPos++;
            if(curPos < expr.size()) {
                const char c = expr[curPos];
                if(!(c == '+' || c == '-' || c == '*' || c == '/')) {
                    throw std::invalid_argument("Invalid operand: " + c);
                }
                oper = c;
            }
            else
                throw std::invalid_argument("Missing operand");


            curPos++;
            if(curPos < expr.size()) {
                const char c = expr[curPos];
                if(c != ' ')
                    throw std::invalid_argument("Invalid symbol after operand: " + c);
            }
            else
                throw std::invalid_argument("Missing space after operand");
            curPos++;

            for(; curPos < expr.size(); curPos++) {
                const char c = expr[curPos];
                num2 += c;
            }
            ComplexNumber n1, n2;
            try {
                n1 = ComplexNumber(num1);
            }
            catch (...) {
                //TODO ошибка глушится
                throw std::invalid_argument("Unable to parse first number");
            }
            try {
                n2 = ComplexNumber(num2);
            }
            catch (...) {
                //TODO ошибка глушится
                throw std::invalid_argument("Unable to parse second number");
            }
            switch (oper) {
                case '-':
                    return n1 - n2;
                case '+':
                    return n1 + n2;
                case '*':
                    return n1 * n2;
                case '/':
                    return n1 / n2;
                default:
                    throw std::invalid_argument("Invalid operand: " + oper);
            }
        }

        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(expr);

        while (std::getline(tokenStream, token, ' ')) {
            tokens.push_back(token);
        }

        ComplexNumber num1, num2;
        int start = 0;
        bool firstFound = false,
            secondFound = false;
        for(int i = 0; i < expr.size(); i++) {
            char c = expr[i];
            if(c == ' ') {

            }
            if(!firstFound) {

            }
        }
    }
}
