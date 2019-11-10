/* This file defines some operations of class BigNum */
#include <iostream>
#include "Bignum.h"

// define the default conductor of class Bignum
// the default value is 0
Bignum::Bignum()
{
    for(int i = 0; i < MAXBITS; i++) val[i] = '0';
}

// define the conductor of class Bignum
// set the val to num, note: num >= 0
Bignum::Bignum(long long num)
{
    long long temp;     // store |num|
    int rem;            // 余数

    // set temp = |num|
    temp = (num < 0) ? (-num) : num;
    // 短除法变换
    for(int i = 0; i < MAXBITS; i++)
    {
        rem = temp % 2;
        temp = temp / 2;
        val[i] = '0' + rem;
    }
}

// define the destructor of class Bignum
Bignum::~Bignum()
{
}

// define the show() of class Bignum
void Bignum::show() const
{
    for(int i = MAXBITS - 1; i >= 0; i--) std::cout<<this->val[i];
}

// define the operator ~ of class Bignum
Bignum Bignum::operator~() const
{
    Bignum result;

    for(int i = 0; i < MAXBITS; i++)
        result.val[i] = (this->val[i] == '0') ? '1' : '0';

    return result;
}

// define the operator left shift << of class Bignum
Bignum Bignum::operator<<(int bits) const
{
    Bignum result;
    // left shift
    for(int i = MAXBITS - 1; i >= bits; i--)
        result.val[i] = this->val[i - bits];
    // fill the blank with zero
    for(int i = 0; i < bits && i < MAXBITS; i++) result.val[i] = '0';
    // return
    return result;
}

// definr the operator algorithm right shift >> of class Bignum
Bignum Bignum::operator>>(int bits) const
{
    Bignum result;
    // right shift
    for(int i = 0; i < MAXBITS - 1 - bits; i++)
        result.val[i] = this->val[i + bits];
    // fill the blank with sign bit
    int begin = (MAXBITS - bits > 0) ? (MAXBITS - bits) : 0;
    for(int i = begin; i < MAXBITS; i++) result.val[i] = this->val[MAXBITS - 1];
    // return
    return result;
}

// define the operator + of class Bignum
Bignum Bignum::operator+(const Bignum & t) const
{
    Bignum sum;      // the sum
    int a, b;        // two addends
    int s;           // the sum bit
    int c = 0;       // the carry bit
    // do addition
    for(int i = 0; i < MAXBITS; i++) {
        a = val[i] - '0';
        b = t.val[i] - '0';
        // compute: s = a + b + c, equal to s = a ^ b ^ c
        s = a ^ b ^ c;
        // write to sum.val[i]
        sum.val[i] = '0' + s;
        // update the carry bit
        c = a&b | b&c | c&a;
    }
    return sum;
}

// define the operator - of class Bignum
Bignum Bignum::operator-(const Bignum & t) const
{
    Bignum one(1);
    // a - b, is equal to a + ~b + 1
    return (*this) + ~t + one;
}

// define the operator == of class Bignum
bool Bignum::operator==(const Bignum & t) const
{
    for(int i = MAXBITS - 1; i >= 0; i--)
        if(this->val[i] != t.val[i]) return false;
    return true;
}

// define the operator < of class Bignum
bool Bignum::operator<(const Bignum & t) const
{
    for(int i = MAXBITS - 1; i >= 0; i--) {
        // this[i] == 0 < t[i] == 1
        if(this->val[i] < t.val[i])      return true;
        // this[i] == 1 > t[i] == 0
        else if(this->val[i] > t.val[i]) return false;
        // this[i] == t[i]
        else ;
    }
    return false;
}

// define the operator > of class Bignum
bool Bignum::operator>(const Bignum & t) const
{
    for(int i = MAXBITS - 1; i >= 0; i--) {
        // this[i] == 0 < t[i] == 1
        if(this->val[i] < t.val[i])      return false;
        // this[i] == 1 > t[i] == 0
        else if(this->val[i] > t.val[i]) return true;
        // this[i] == t[i]
        else ;
    }
    return false;
}

// define the operator * of class Bignum
Bignum Bignum::operator*(const Bignum & t) const
{
    Bignum result(0);
    Bignum temp;

    // do multiplication
    temp = *this;
    for(int i = 0; i < MAXBITS; i++) {
        // if t[i] == '0', result = result + 0
        if(t.val[i] == '0');
        // if t[i] == '1', result = result + (*this) << i
        else result = result + temp;
        // update temp
        temp = temp << 1;
    }

    // return
    return result;
}

// define the operator / of class Bignum
Bignum Bignum::operator/(const Bignum & t) const
{
    Bignum R(0);        // initial value: 0 extend of dividend
    Bignum Q = *this;   // initial value: dividend
    Bignum quo;         // quotient

    // do division
    for(int k = 0; k < MAXBITS; k++) {
        // {R, Q} << 1
        for(int i = MAXBITS - 1; i > 0; i--) R.val[i] = R.val[i - 1];
        R.val[0] = Q.val[MAXBITS - 1];
        for(int i = MAXBITS - 1; i > 0; i--) Q.val[i] = Q.val[i - 1];
        Q.val[0] = '0';
        // if R < t, quo[M - 1 - k] = 0
        if(R < t) {
            quo.val[MAXBITS - 1 - k] = '0';
        }
        // if R >= t, quo[M - 1 - k] = 0, R = R -t
        else {
            quo.val[MAXBITS - 1 - k] = '1';
            R = R - t;
        }
    }

    // return
    return quo;
}

// define the operator % of class Bignum
Bignum Bignum::operator%(const Bignum & t) const
{
    Bignum R(0);        // initial value: 0, final value: remainder
    Bignum Q = *this;   // initial value: dividend, final value: 0
    Bignum quo;         // quotient

    // do division
    for(int k = 0; k < MAXBITS; k++) {
        // {R, Q} << 1
        for(int i = MAXBITS - 1; i > 0; i--) R.val[i] = R.val[i - 1];
        R.val[0] = Q.val[MAXBITS - 1];
        for(int i = MAXBITS - 1; i > 0; i--) Q.val[i] = Q.val[i - 1];
        Q.val[0] = '0';
        // if R < t, quo[M - 1 - k] = 0
        if(R < t) {
            quo.val[MAXBITS - 1 - k] = '0';
        }
        // if R >= t, quo[M - 1 - k] = 0, R = R -t
        else {
            quo.val[MAXBITS - 1 - k] = '1';
            R = R - t;
        }
    }

    // return
    return R;
}

