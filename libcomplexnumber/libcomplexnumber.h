#ifndef LIBCOMPLEXNUMBER_FRACTIONNUM_H
#define LIBCOMPLEXNUMBER_FRACTIONNUM_H
#include <ostream>

namespace libcomplexnumber {
    struct FractionNum {
    private:
        long numerator;
        long denominator;

        static void simplify(long& numerator_, long& denominator_);

    public:
        FractionNum();
        explicit FractionNum(long numerator_, long denominator_);
        static FractionNum CreateFractionNum(long numerator_, long denominator_);

        explicit operator double() const;

        FractionNum operator/ (const FractionNum& other) const;
        FractionNum operator* (const FractionNum& other) const;
        bool operator== (const FractionNum& other) const;
        FractionNum operator+ (const FractionNum& other) const;
        FractionNum operator- (const FractionNum& other) const;
        friend std::ostream& operator<< (std::ostream& os, const FractionNum& num);
    };

    struct ComplexNumber {
    private:
        FractionNum coefficient;
        FractionNum power;
        bool pInPower;

    public:
        explicit ComplexNumber(FractionNum coefficient_, FractionNum power_, bool pInPower_);
        explicit ComplexNumber(std::string num);
    };


}
#endif // LIBCOMPLEXNUMBER_FRACTIONNUM_H