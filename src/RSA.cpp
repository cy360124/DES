/* This file defines some functions of RSA algorithm */
#include <stdlib.h>
#include <time.h>
#include "global.h"
#include "Bignum.h"

// const values
const Bignum zero(0);
const Bignum one(1);
const Bignum two(2);

// RSA Keys
Bignum RSA_N;                   // the N of RSA
Bignum RSA_D;                   // the D of RSA
Bignum RSA_E;                   // the E of RSA

// conduct the inf = {1/4 * 0, 3/4 * 1}
Bignum conduct_inf()
{
	Bignum result;
	char temp[RSABITS];
	for (int i = 0; i < RSABITS / 4; i++) temp[i] = '1';
	for (int i = RSABITS / 4; i < RSABITS; i++) temp[i] = '0';
	result.Char2Bignum(temp, RSABITS);
	return result;
}

// compute the greatest common divisor, with extension
// return gcd(x, n), and modify y such that (x * y) mod n = 1
Bignum extend_gcd(Bignum n, Bignum x, Bignum & y)
{
	Bignum rem;                // remainder
	Bignum temp;               // store middle value
	Bignum pn = n;             // store the initial value of n
	Bignum t0(0), t1(1), t2;   // t_array, t0 = 0, t1 = 1
	// Õ·×ªÏà³ý·¨
	while (x != zero) {
		// compute gcd
		rem = n % x;
		temp = n / x;
		n = x;
		x = rem;
		// compute y = t[m]
		t2 = pn + t0 % pn- (temp * t1) % pn;
		t0 = t1; t1 = t2;
	}
	y = t0 % pn;
	return n;
}

// compute: pow(base, power) % mod
Bignum power_mod(const Bignum & base, const Bignum & power, const Bignum & mod)
{
    static Bignum pre[RSABITS];      // store the value of pow(base, 2^i) % mod
    char pow[RSABITS];               // store the bits of power
    Bignum result(1);                // store reuslt, initial value: 1

    // prepare work
    pre[0] = base % mod;
    for (int i = 1; i < RSABITS; i++) pre[i] = (pre[i - 1] * pre[i - 1]) % mod;

    // read power into pow[]
    power.Bignum2Char(pow, RSABITS);

    // for each bit of pow
    for (int i = 0; i < RSABITS; i++) {
        if (pow[i] == '0') continue;
        else {
            result = (result * pre[i]) % mod;
        }
    }

    // return
    return result;
}

// conduct a random Bignum in 64 bits
Bignum random_num()
{
    char temp[RSABITS];
    Bignum result;

    // conduct random temp[]
    srand((unsigned)time(NULL));
    for (int i = 0; i < RSABITS/2; i++) temp[i] = (rand() % 2) + '0';
    for (int i = RSABITS/2; i < RSABITS; i++) temp[i] = '0';

    // conduct Bignum result
    result.Char2Bignum(temp, RSABITS);

    // return
    return result;
}

// test whether n is a prime number
bool test_prime(int k, const Bignum & a, const Bignum & m, const Bignum & n)
{
    Bignum b;

    // compute b = pow(a, m) % n
    b = power_mod(a, m, n);

    // if b % n == 1, then n may be a prime number
    if (b % n == one) return true;

    // else, then from 0 to k-1, check it
    for (int i = 0; i < k; i++) {
        // if b % n == -1, then n may be a prime number
        if (b % n == n - one) return true;
        // else, update b
        b = (b * b) % n;
    }

    // if not return before, then n is not a prime number
    return false;
}

// use Miller-Rabin method to check whether n is a prime number
bool Miller_Rabin(const Bignum & n)
{
    Bignum a;              // random Bignum
	Bignum m;              // m = n - 1
    int k = 0;             // power value
    bool mark;             // just a mark

    // if n is not a odd number
    if(n % two == zero) return false;

    // for n-1 = 2^k * m, compute k & m
	m = n - one;
    while (m % two == zero) {
        m = m / two;
        k++;
    }

    // do tests
    for (int i = 0; i < TESTTIMES; i++) {
        // produce random Bignum
        a = random_num() % n;
        // test
        mark = test_prime(k, a, m, n);
        // if test is true, continue until TESTTIMES
        if (mark == true) continue;
        // once n is not a prime number, return false
        else             return false;
    }

    // n is a prime number
    return true;
}

// load RSA, which produce public key and private key
void load_RSA()
{
    Bignum inf;
    Bignum RSA_P, RSA_Q, fn;

	// conduct the inf
	inf = conduct_inf();

    // conduct prime P and prime Q
	RSA_P = inf + random_num();
    while (1) {
        if(Miller_Rabin(RSA_P)) break;
		RSA_P = RSA_P + one;
    }
	RSA_Q = inf + random_num();
	while (1) {
		if (Miller_Rabin(RSA_Q)) break;
		RSA_Q= RSA_Q + one;
	}

    // compute N and fn
    RSA_N = RSA_P * RSA_Q;
    fn = (RSA_P - one) * (RSA_Q - one);

    // conduct D: gcd(D, fn) == 1
    // conduct E: D * E mod fn = 1
	RSA_D = inf;
    while (1) {
		if (extend_gcd(fn, RSA_D, RSA_E) == one) break;
		RSA_D = RSA_D + one;
    }
}

// achieve the encryption of primaryKey in RSA.
// des[] should be 128-bit, src[] should be 64-bit
void RSA_encryption(char des[], const char src[])
{
	Bignum message, cipher;
	// conduct message
	message.Char2Bignum(src, 64);
	// compute cipher
	cipher = power_mod(message, RSA_E, RSA_N);
	// conduct des[]
	cipher.Bignum2Char(des, RSABITS);
}

// achieve the decryption of primaryKey in RSA.
// des[] should be 64-bit, src[] should be 128-bit
void RSA_decryption(char des[], const char src[])
{
	Bignum message, cipher;
	// conduct cipher
	cipher.Char2Bignum(src, RSABITS);
	// compute message
	message = power_mod(cipher, RSA_D, RSA_N);
	// conduct des[]
	message.Bignum2Char(des, 64);
}
