#ifndef LIBCOMPLEXNUMBER_H
#define LIBCOMPLEXNUMBER_H
#include <ostream>

namespace libcnum {
    struct FractionNum {
    private:
        long numerator;
        long denominator;

        static void simplify(long& numerator_, long& denominator_);
        explicit FractionNum();
        explicit FractionNum(long numerator_, long denominator_);

    public:
        explicit operator double() const;

        FractionNum operator/ (const FractionNum& other) const;
        FractionNum operator* (const FractionNum& other) const;
        bool operator== (const FractionNum& other) const;
        FractionNum operator+ (const FractionNum& other) const;
        FractionNum operator- (const FractionNum& other) const;
        friend std::ostream& operator<< (std::ostream& os, const FractionNum& num);


        bool assert_test(long cNum, long cDen) const {
            simplify(cNum, cDen);
            return numerator == cNum && denominator == cDen;
        }
        friend struct ComplexNumber;
        friend FractionNum CreateFractionNum(long numerator_, long denominator_);
    };

    struct ComplexNumber {
    private:
        FractionNum coefficient;
        FractionNum power;
        bool pInPower;

    public:
        explicit ComplexNumber(FractionNum coefficient_, FractionNum power_, bool pInPower_);
        explicit ComplexNumber(std::string num);
        explicit ComplexNumber();

        //Штука для тестов парсинга при компиляции
        bool assert_test(const long cNum, const long cDen, const long pNum, const long pDen, const bool pPow) const {
            return coefficient.assert_test(cNum, cDen) && power.assert_test(pNum, pDen) && pInPower == pPow;
        }
    };

    FractionNum CreateFractionNum(long numerator_, long denominator_);




}
#endif // LIBCOMPLEXNUMBER_H