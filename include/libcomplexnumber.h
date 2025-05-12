#ifndef LIBCOMPLEXNUMBER_H
#define LIBCOMPLEXNUMBER_H
#include <ostream>

namespace libcnum {
    struct FractionNum {
    private:
        bool pInNumerator;
        long numerator;
        long denominator;

        static void simplify(long& numerator_, long& denominator_);
        explicit FractionNum();
        explicit FractionNum(long numerator_, long denominator_, bool pInNumerator_);

    public:
        explicit operator double() const;

        FractionNum operator/ (const FractionNum& other) const;
        FractionNum operator* (const FractionNum& other) const;
        bool operator== (const FractionNum& other) const;
        FractionNum operator+ (const FractionNum& other) const;
        FractionNum operator- (const FractionNum& other) const;
        friend std::ostream& operator<< (std::ostream& os, const FractionNum& num);


        bool assert_test(long cNum, long cDen, bool pNum) const {
            simplify(cNum, cDen);
            return numerator == cNum && denominator == cDen && pInNumerator == pNum;
        }
        friend struct ComplexNumber;
        friend FractionNum CreateFractionNum(long numerator_, long denominator_, bool pInNumerator_);
    };

    struct ComplexNumber {
    private:
        FractionNum coefficient;
        FractionNum power;

    public:
        explicit ComplexNumber(FractionNum coefficient_, FractionNum power_);
        explicit ComplexNumber(std::string num);
        explicit ComplexNumber();

        //Штука для тестов парсинга при компиляции
        bool assert_test(const long cNum, const long cDen, const long pNum, const long pDen, const bool pPow) const {
            return coefficient.assert_test(cNum, cDen, false) && power.assert_test(pNum, pDen, pPow);
        }
    };

    FractionNum CreateFractionNum(long numerator_, long denominator_, bool pInNumerator_);



}
#endif // LIBCOMPLEXNUMBER_H