#ifndef _BIGINT_H_
#define _BIGINT_H_

#include "biguint.h"

//---------------------------------------------------------------
class bigint_t
{
public:
	int       sign;
	biguint_t mag;

	bigint_t ( void );
	bigint_t ( const bigint_t& x );
	bigint_t& operator=( const bigint_t& x );

	bigint_t ( const biguint_t& x );
	bigint_t ( const biguint_t& x, int s );

	bigint_t ( const unsigned long *b, unsigned int len );
	bigint_t ( const unsigned long *b, unsigned int len, int sgn );

	bigint_t ( long x );
	bigint_t ( int x );
	bigint_t ( short x );

	bigint_t ( unsigned long x );
	bigint_t ( unsigned int x );
	bigint_t ( unsigned short x );

	long  to_long  ( void ) const;
	int   to_int   ( void ) const;
	short to_short ( void ) const;

	unsigned long  to_ulong ( void ) const;
	unsigned int   to_uint  ( void ) const;
	unsigned short to_ushort( void ) const;

	int      compareTo  ( const bigint_t& x ) const;
	bool     operator== ( const bigint_t& x ) const;
	bool     operator!= ( const bigint_t& x ) const;
	bool     operator<  ( const bigint_t& x ) const;
	bool     operator>= ( const bigint_t& x ) const;
	bool     operator>  ( const bigint_t& x ) const;
	bool     operator<= ( const bigint_t& x ) const;

	bigint_t operator+  ( const bigint_t& x ) const;
	bigint_t operator-  ( const bigint_t& x ) const;
	bigint_t operator*  ( const bigint_t& x ) const;
	bigint_t operator/  ( const bigint_t& x ) const;
	bigint_t operator%  ( const bigint_t& x ) const;
	bigint_t operator-  ( void ) const;

	void     operator+= ( const bigint_t& x );
	void     operator-= ( const bigint_t& x );
	void     operator*= ( const bigint_t& x );
	void     operator/= ( const bigint_t& x );
	void     operator%= ( const bigint_t& x );

	void     operator++ (     );
	void     operator++ ( int );
	void     operator-- (     );
	void     operator-- ( int );

	void     negate     (const bigint_t& a);
	void     add        (const bigint_t& a, const bigint_t& b);
	void     subtract   (const bigint_t& a, const bigint_t& b);
	void     multiply   (const bigint_t& a, const bigint_t& b);
	void     divide     (const bigint_t& b, bigint_t& q);

	      bool       isNegative  ( void ) const;
	const biguint_t& getMagnitude( void ) const;
	      unsigned long getBlock    ( unsigned int i ) const;

	template <class X>           X convertToUnsignedPrimitive() const;
	template <class X, class UX> X convertToSignedPrimitive() const;
};

//------------------------------------------------------------
inline void bigint_t::operator++(int) { operator++(); }
inline void bigint_t::operator++(void)
{
	if (sign == -1)
	{
		mag--;
		if (mag == 0)
			sign = 0;
	} else
	{
		mag++;
		sign = 1;
	}
}

//------------------------------------------------------------
inline void bigint_t::operator--(int) { operator--(); }
inline void bigint_t::operator--(void)
{
	if (sign == 1)
	{
		mag--;
		if (mag == 0)
			sign = 0;
	} else
	{
		mag++;
		sign = -1;
	}
}

//------------------------------------------------------------
inline bool bigint_t::operator!=(const bigint_t &x) const
{
	return !operator==(x);
}

//------------------------------------------------------------
inline bool bigint_t::operator<(const bigint_t &x) const
{
	return compareTo(x) < 0;
}

//------------------------------------------------------------
inline bool bigint_t::operator<=(const bigint_t &x) const
{
	return compareTo(x) <= 0;
}

//------------------------------------------------------------
inline bool bigint_t::operator>(const bigint_t &x) const
{
	return compareTo(x) > 0;
}

//------------------------------------------------------------
inline bool bigint_t::operator>=(const bigint_t &x) const
{
	return compareTo(x) >= 0;
}

//------------------------------------------------------------
inline bool bigint_t::operator==(const bigint_t &x) const
{
	return (sign == x.sign) && (mag == x.mag);
}

//------------------------------------------------------------
inline bigint_t bigint_t::operator+(const bigint_t &x) const
{
	bigint_t ans;
	ans.add(*this, x);
	return ans;
}

//------------------------------------------------------------
inline bigint_t bigint_t::operator-(const bigint_t &x) const
{
	bigint_t ans;
	ans.subtract(*this, x);
	return ans;
}

//------------------------------------------------------------
inline bigint_t bigint_t::operator*(const bigint_t &x) const
{
	bigint_t ans;
	ans.multiply(*this, x);
	return ans;
}

//------------------------------------------------------------
inline bigint_t bigint_t::operator/(const bigint_t &x) const
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (sign==0)
		throw "bigint_t::operator /: division by zero";
	bigint_t q, r;
	r = *this;
	r.divide(x, q);
	return q;
}

//------------------------------------------------------------
inline bigint_t bigint_t::operator%(const bigint_t &x) const
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (x.sign==0)
		throw "bigint_t::operator %: division by zero";
	bigint_t q, r;
	r = *this;
	r.divide(x, q);
	return r;
}

//------------------------------------------------------------
inline bigint_t bigint_t::operator-() const
{
	bigint_t ans;
	ans.negate(*this);
	return ans;
}

//------------------------------------------------------------
inline void bigint_t::operator+=(const bigint_t &x)
{
	add(*this, x);
}

//------------------------------------------------------------
inline void bigint_t::operator-=(const bigint_t &x)
{
	subtract(*this, x);
}

//------------------------------------------------------------
inline void bigint_t::operator*=(const bigint_t &x)
{
	multiply(*this, x);
}

//------------------------------------------------------------
inline void bigint_t::operator/=(const bigint_t &x)
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (x.sign==0)
		throw "bigint_t::operator /=: division by zero";

	bigint_t q;
	divide(x, q);
	*this = q;
}

//------------------------------------------------------------
inline void bigint_t::operator %=(const bigint_t &x)
{
	//TODO: Why not use mag.len==0 for test for zero to be consistent?
	if (x.sign==0)
		throw "bigint_t::operator %=: division by zero";
	bigint_t q;
	divide(x, q);
}

#endif
