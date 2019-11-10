#ifndef CODE_BIGNUM_H
#define CODE_BIGNUM_H

// the class Bignum is unsigned big integer
class Bignum
{
private:
    enum {MAXBITS = 128};   // the length of val[]
    char val[MAXBITS];      // the value in binary complement, note: 0 -> MAXBITS is low -> high
public:
    Bignum();                 // default conductor
    Bignum(long long num);          // conductor
    ~Bignum();                // destructor
    void show() const;
    Bignum operator~() const;
    Bignum operator<<(int bits) const;
    Bignum operator>>(int bits) const;
    Bignum operator+(const Bignum & t) const;
    Bignum operator-(const Bignum & t) const;
    bool operator==(const Bignum & t) const;
    bool operator<(const Bignum & t) const;
    bool operator>(const Bignum & t) const;
    Bignum operator*(const Bignum & t) const;
    Bignum operator/(const Bignum & t) const;
    Bignum operator%(const Bignum & t) const;
};

#endif  /* !CODE_BIGNUM_H */
