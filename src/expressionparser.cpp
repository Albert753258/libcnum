#include "../include/libcnum.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>


namespace libcnum {
    ComplexNumber calculateExponentialExpression(const std::string &expr) {
        int curPos = 0;
        if(expr[curPos] != '('){
            throw std::invalid_argument("unable to find opening bracket");
        }

        curPos = 1;
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
            throw std::invalid_argument("unable to find closing bracket");
        ComplexNumber n1;
        try {
            n1 = ComplexNumber(num1);
        }
        catch (const std::exception& err) {
            throw std::invalid_argument(std::string("error parsing complex number ") + num1 + ": " + err.what());
        }

        curPos++;
        if(curPos < expr.size()) {
            const char c = expr[curPos];
            if(c != '^') {
                throw std::invalid_argument(std::string("invalid symbol after closing bracket (should be ^): ") + c);
            }
        }
        else
            throw std::invalid_argument("unable to find ^ after closing bracket");
        curPos++;

        long buf = 0;

        for(; curPos < expr.length(); curPos++) {
            const char c = expr[curPos];
            if(c < '0' or c > '9') {
                throw std::invalid_argument(std::string("invalid symbol in power: ") + c);
            }

            buf *= 10;
            buf += c - '0';
        }
        if(buf == 0)
            throw std::invalid_argument("unable to find power");

        return n1.pow(buf);
    }
    ComplexNumber calculateSimpleExpression(const std::string &expr) {
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
            throw std::invalid_argument("unable to find first operand");
        curPos++;
        if(curPos < expr.size()) {
            const char c = expr[curPos];
            if(!(c == '+' || c == '-' || c == '*' || c == '/')) {
                throw std::invalid_argument(std::string("invalid operand: ") + c);
            }
            oper = c;
        }
        else
            throw std::invalid_argument("missing operand");


        curPos++;
        if(curPos < expr.size()) {
            const char c = expr[curPos];
            if(c != ' ')
                throw std::invalid_argument(std::string("invalid symbol after operand: ") + c);
        }
        else
            throw std::invalid_argument("missing space after operand");
        curPos++;

        for(; curPos < expr.size(); curPos++) {
            const char c = expr[curPos];
            num2 += c;
        }
        ComplexNumber n1, n2;
        try {
            n1 = ComplexNumber(num1);
        }
        catch (const std::exception& err) {
            throw std::invalid_argument(std::string("unable to parse first number: ") + err.what());
        }
        try {
            n2 = ComplexNumber(num2);
        }
        catch (const std::exception& err) {
            throw std::invalid_argument(std::string("unable to parse second number: ") + err.what());
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
            // ReSharper disable once CppDFAUnreachableCode
            default:
                throw std::invalid_argument(std::string("invalid operand: ") + oper);
        }
    }

    ComplexNumber calculateExpression(const std::string &expr) {
        if(expr.empty()) {
            throw std::invalid_argument(std::string("Expression is empty"));
        }
        else if(expr[0] == '(') {
            try {
                return calculateExponentialExpression(expr);
            }
            catch (const std::exception& err) {
                throw std::invalid_argument(std::string("Unable to pow: ") + err.what());
            }
        }
        else {
            try {
                return calculateSimpleExpression(expr);
            }
            catch (const std::exception& err) {
                throw std::invalid_argument(std::string("Unable calculate expression: ") + err.what());
            }
        }
    }
}
