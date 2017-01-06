#ifndef _BIGLIB_H_
#define _BIGLIB_H_

#include <string.h>
#include <iostream>
#include "biguint.h"
#include "bigint.h"

using namespace std;

extern       string     to_string         (const biguint_t& bu);
extern       string     to_hex            (const biguint_t& bu);
extern       string     to_string         (const bigint_t&  bi);

extern       biguint_t  str2BigUint       (const string& s);
extern       bigint_t   str2BigInt        (const string& s);

extern       biguint_t  hex2BigUint       (const string& s);
extern       bigint_t   hex2BigInt        (const string& s);

extern       biguint_t  exp2BigUint       (const string& s);
extern       bigint_t   exp2BigInt        (const string& s);

extern       ostream&   operator<<        (ostream& os, const biguint_t& x);
extern       ostream&   operator<<        (ostream& os, const bigint_t& x);

extern       biguint_t  gcd               (biguint_t a, biguint_t b);
extern       biguint_t  modinv            (const bigint_t& x, const biguint_t& n);
extern       biguint_t  modexp            (const bigint_t& base, const biguint_t& exponent,const biguint_t& modulus);

#endif
