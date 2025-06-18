#ifndef LIBCNUM_H
#define LIBCNUM_H
#include <ostream>

namespace libcnum {
    struct ComplexNumber;
    struct FractionNum {
    private:
        bool pInNumerator;
        long numerator;
        long denominator;

        static void simplify(long& numerator_, long& denominator_);
        explicit FractionNum();
        explicit FractionNum(long numerator_, long denominator_, bool pInNumerator_);

    public:
        explicit FractionNum(double num);
        explicit operator double() const;

        FractionNum operator/ (const FractionNum& other) const;
        FractionNum operator* (const FractionNum& other) const;
        FractionNum operator* (const long& other) const;
        bool operator== (const FractionNum& other) const;
        bool operator== (const long& other) const;
        bool operator!= (const long& other) const;
        FractionNum operator+ (const FractionNum& other) const;
        FractionNum operator- (const FractionNum& other) const;
        [[nodiscard]] std::string ToString() const;
        friend std::ostream& operator<< (std::ostream& os, const FractionNum& num);


        [[nodiscard]] bool assert_test(long cNum, long cDen, const bool pNum) const {
            simplify(cNum, cDen);
            return numerator == cNum && denominator == cDen && pInNumerator == pNum;
        }
        friend struct ComplexNumber;
        friend std::ostream& operator<< (std::ostream& os, const ComplexNumber& num);
        friend FractionNum CreateFractionNum(long numerator_, long denominator_, bool pInNumerator_);
    };

    struct ComplexNumber {
    private:
        FractionNum coefficient;
        FractionNum power;

    public:
        explicit ComplexNumber(const FractionNum &coefficient_, const FractionNum &power_);
        explicit ComplexNumber(const std::string &num);
        explicit ComplexNumber();


        ComplexNumber operator/ (const ComplexNumber& other) const;
        ComplexNumber operator* (const ComplexNumber& other) const;
        bool operator== (const ComplexNumber& other) const;
        ComplexNumber operator+ (const ComplexNumber& other) const;
        ComplexNumber operator- (const ComplexNumber& other) const;
        [[nodiscard]] ComplexNumber pow(long power) const;
        friend std::ostream& operator<< (std::ostream& os, const ComplexNumber& num);

        [[nodiscard]] std::string ToExponential() const;
        [[nodiscard]] std::string ToAlgebraic() const;
        [[nodiscard]] std::string ToTriganometric() const;

        //Штука для тестов парсинга при компиляции
        [[nodiscard]] bool assert_test(const long cNum, const long cDen, const long pNum, const long pDen, const bool pPow) const {
            return coefficient.assert_test(cNum, cDen, false) && power.assert_test(pNum, pDen, pPow);
        }
    };

    FractionNum CreateFractionNum(long numerator_, long denominator_, bool pInNumerator_);

    ComplexNumber calculateExpression(const std::string &expr);

}
#endif // LIBCNUM_H