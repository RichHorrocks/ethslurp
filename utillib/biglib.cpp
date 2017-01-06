#include "biglib.h"

/*
 * A BigUnsignedInABase object represents a nonnegative integer of size limited
 * only by available memory, represented in a user-specified base that can fit
 * in an `unsigned short' (most can, and this saves memory).
 *
 * BigUnsignedInABase is intended as an intermediary class with little
 * functionality of its own.  BigUnsignedInABase objects can be constructed
 * from, and converted to, biguint_ts (requiring multiplication, mods, etc.)
 * and `string's (by switching igit values for appropriate characters).
 *
 * BigUnsignedInABase is similar to biguint_t.  Note the following:
 *
 * (1) They represent the number in exactly the same way, except that
 * BigUnsignedInABase uses ``igits'' (or igit) where biguint_t uses
 * ``blocks'' (or Blk).
 *
 * (2) Both use the management features of BigNumStore.  (In fact, my desire
 * to add a BigUnsignedInABase class without duplicating a lot of code led me to
 * introduce BigNumStore.)
 *
 * (3) The only arithmetic operation supported by BigUnsignedInABase is an
 * equality test.  Use biguint_t for arithmetic.
 */

class BigUnsignedInABase : public BigNumStore<unsigned short>
{
public:
	unsigned short base;

	// Creates a BigUnsignedInABase with a capacity; for internal use.
	BigUnsignedInABase(int, unsigned int c) : BigNumStore<unsigned short>(0, c) {}

	// Decreases len to eliminate any leading zero igits.
	void trimLeft()
	{
		while (len > 0 && blk[len - 1] == 0)
			len--;
	}

	// Constructs zero in base 2.
	BigUnsignedInABase() : BigNumStore<unsigned short>(), base(2) {}

	// Copy constructor
	BigUnsignedInABase(const BigUnsignedInABase &x) : BigNumStore<unsigned short>(x), base(x.base) {}

	// Assignment operator
	void operator =(const BigUnsignedInABase &x) {
		BigNumStore<unsigned short>::operator =(x);
		base = x.base;
	}

	// Constructor that copies from a given array of igits.
	BigUnsignedInABase(const unsigned short *d, unsigned int l, unsigned short base);

	// Destructor.  BigNumStore does the delete for us.
	~BigUnsignedInABase() {}

	// LINKS TO BIGUNSIGNED
	BigUnsignedInABase(const biguint_t &x, unsigned short base);
	operator biguint_t() const;

	operator string() const;
	BigUnsignedInABase(const string &s, unsigned short base);

	/*
	* Equality test.  For the purposes of this test, two BigUnsignedInABase
	* values must have the same base to be equal.
	*/
	bool operator ==(const BigUnsignedInABase &x) const {
		return base == x.base && BigNumStore<unsigned short>::operator ==(x);
	}
	bool operator !=(const BigUnsignedInABase &x) const { return !operator ==(x); }

};

inline BigUnsignedInABase::BigUnsignedInABase(const unsigned short *d, unsigned int l, unsigned short base)
: BigNumStore<unsigned short>(d, l), base(base)
{
        if (base < 2)
                throw "BigUnsignedInABase::BigUnsignedInABase(const unsigned short *, unsigned int, unsigned short): The base must be at least 2";

        for (unsigned int i = 0; i < l; i++)
                if (blk[i] >= base)
                        throw "BigUnsignedInABase::BigUnsignedInABase(const unsigned short *, unsigned int, unsigned short): A d igit is too large for the specified base";

        trimLeft();
}

//------------------------------------------------------------------
namespace
{
	unsigned int bitLen(unsigned int x)
	{
		unsigned int len = 0;
		while (x > 0)
		{
			x >>= 1;
			len++;
		}
		return len;
	}
}

//------------------------------------------------------------------
BigUnsignedInABase::BigUnsignedInABase(const biguint_t &x, unsigned short base)
{
	if (base < 2)
		throw "BigUnsignedInABase(biguint_t, Base): The base must be at least 2";
	this->base = base;

	// Get an upper bound on how much space we need
	int maxBitLenOfX    = x.len * biguint_t::N;
	int minBitsPerDigit = bitLen(base) - 1;
	int maxDigitLenOfX  = (maxBitLenOfX + minBitsPerDigit - 1) / minBitsPerDigit;

	len = maxDigitLenOfX; // Another change to comply with `staying in bounds'.
	allocate(len); // Get the space

	biguint_t x2(x), buBase(base);
	unsigned int digitNum = 0;

	while (x2.len!=0)
	{
		// Get last digit.  This is like `lastDigit = x2 % buBase, x2 /= buBase'.
		biguint_t lastDigit(x2);
		lastDigit.divide(buBase, x2);
		// Save the digit.
		blk[digitNum] = lastDigit.to_ushort();
		// Move on.  We can't run out of room: we figured it out above.
		digitNum++;
	}

	// Save the actual length.
	len = digitNum;
}

//------------------------------------------------------------------
BigUnsignedInABase::BigUnsignedInABase(const string &s, unsigned short base)
{
	if (base > 36)
		throw "BigUnsignedInABase(string, unsigned short): The default string conversion routines use the symbol set 0-9, A-Z and therefore support only up to base 36.  You tried a conversion with a base over 36; write your own string conversion routine.";

	this->base = base;

	// `s.length()' is a `size_t', while `len' is a unsigned int,
	// also known as an `unsigned int'.  Some compilers warn without this cast.
	len = (unsigned int)(s.length());
	allocate(len);

	unsigned int digitNum, symbolNumInString;
	for (digitNum = 0; digitNum < len; digitNum++)
	{
		symbolNumInString = len - 1 - digitNum;
		char theSymbol = s[symbolNumInString];

		if (theSymbol >= '0' && theSymbol <= '9')
			blk[digitNum] = theSymbol - '0';

		else if (theSymbol >= 'A' && theSymbol <= 'Z')
			blk[digitNum] = theSymbol - 'A' + 10;

		else if (theSymbol >= 'a' && theSymbol <= 'z')
			blk[digitNum] = theSymbol - 'a' + 10;

		else
			throw "BigUnsignedInABase(string, unsigned short): Bad symbol in input.  Only 0-9, A-Z, a-z are accepted.";

		if (blk[digitNum] >= base)
			throw "BigUnsignedInABase::BigUnsignedInABase(const unsigned short *, unsigned int, unsigned short): A digit is too large for the specified base";
	}
	trimLeft();
}

//------------------------------------------------------------------
BigUnsignedInABase::operator biguint_t(void) const
{
	biguint_t ans(0), buBase(base), temp;
	unsigned int digitNum = len;
	while (digitNum > 0)
	{
		digitNum--;
		temp.multiply(ans, buBase);
		ans.add(temp, biguint_t(blk[digitNum]));
	}
	return ans;
}

//------------------------------------------------------------------
BigUnsignedInABase::operator string(void) const
{
	if (len==0)
		return string("0");

	char *s = new char[len+1];
	memset(s,'\0',len+1);
	for (unsigned int p=0;p<len;p++)
	{
		unsigned short c = blk[len-1-p];
		s[p] = ((c < 10) ? char('0'+c) : char('A'+c-10));
	}

	string ret = s;
	delete [] s;
	return ret;
}

//--------------------------------------------------------------------------------
string to_string(const biguint_t& i)
{
	return string(BigUnsignedInABase(i, 10));
}

//--------------------------------------------------------------------------------
string to_hex(const biguint_t& i)
{
	return string(BigUnsignedInABase(i, 16));
}

//--------------------------------------------------------------------------------
string to_string(const bigint_t& i)
{
	return (i.isNegative() ? string("-") : "") + to_string(i.getMagnitude());
}

//--------------------------------------------------------------------------------
biguint_t str2BigUint(const string &s)
{
	return biguint_t(BigUnsignedInABase(s, 10));
}

//--------------------------------------------------------------------------------
biguint_t hex2BigUint(const string &s)
{
	return biguint_t(BigUnsignedInABase(s, 16));
}

//--------------------------------------------------------------------------------
bigint_t str2BigInt(const string &s)
{
	return (s[0] == '-') ? bigint_t(str2BigUint(s.substr(1, s.length() - 1)), -1)
		: (s[0] == '+') ? bigint_t(str2BigUint(s.substr(1, s.length() - 1)))
		: bigint_t(str2BigUint(s));
}

//--------------------------------------------------------------------------------
ostream &operator <<(ostream &os, const biguint_t& x)
{
	unsigned short base;

	long osFlags = os.flags();
	if (osFlags & os.dec)
	{
		base = 10;

	} else if (osFlags & os.hex)
	{
		base = 16;
		if (osFlags & os.showbase)
			os << "0x";

	} else if (osFlags & os.oct)
	{
		base = 8;
		if (osFlags & os.showbase)
			os << '0';
	} else
	{
		throw "ostream << biguint_t: Could not determine the desired base from output-stream flags";
	}
	os << string(BigUnsignedInABase(x, base));
	return os;
}

//--------------------------------------------------------------------------------
ostream &operator <<(ostream &os, const bigint_t &x)
{
	if (x.isNegative())
		os << '-';
	os << x.getMagnitude();
	return os;
}

//--------------------------------------------------------------------------------
biguint_t gcd(biguint_t a, biguint_t b)
{
	biguint_t trash;
	for (;;)
	{
		if (b.len==0)
			return a;
		a.divide(b, trash);
		if (a.len==0)
			return b;
		b.divide(a, trash);
	}
}

//--------------------------------------------------------------------------------
biguint_t modexp(const bigint_t &base, const biguint_t& exponent, const biguint_t& modulus)
{
	biguint_t ans = 1, base2 = (base % modulus).getMagnitude();
	unsigned int i = exponent.bitLength();
	// For each bit of the exponent, most to least significant...
	while (i > 0)
	{
		i--;
		// Square.
		ans *= ans;
		ans %= modulus;
		// And multiply if the bit is a 1.
		if (exponent.getBit(i)) {
			ans *= base2;
			ans %= modulus;
		}
	}
	return ans;
}

//--------------------------------------------------------------------------------
static void exteuclidean(bigint_t m, bigint_t n, bigint_t &g, bigint_t &r, bigint_t &s)
{
	if (&g == &r || &g == &s || &r == &s)
		throw "bigint_t exteuclidean: Outputs are aliased";
	bigint_t r1(1), s1(0), r2(0), s2(1), q;
	/*
	* Invariants:
	* r1*m(orig) + s1*n(orig) == m(current)
	* r2*m(orig) + s2*n(orig) == n(current)
	*/
	for (;;) {
		if (n.sign==0) {
			r = r1; s = s1; g = m;
			return;
		}
		// Subtract q times the second invariant from the first invariant.
		m.divide(n, q);
		r1 -= q*r2; s1 -= q*s2;

		if (m.sign==0) {
			r = r2; s = s2; g = n;
			return;
		}
		// Subtract q times the first invariant from the second invariant.
		n.divide(m, q);
		r2 -= q*r1; s2 -= q*s1;
	}
}

//--------------------------------------------------------------------------------
biguint_t modinv(const bigint_t &x, const biguint_t& n)
{
	bigint_t g, r, s;
	exteuclidean(x, n, g, r, s);
	if (g == 1)
		// r*x + s*n == 1, so r*x === 1 (mod n), so r is the answer.
		return (r % n).getMagnitude(); // (r % n) will be nonnegative
	else
		throw "bigint_t modinv: x and n have a common factor";
}
