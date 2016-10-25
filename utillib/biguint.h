#ifndef _BIG_UNSIGNED_H_
#define _BIG_UNSIGNED_H_

#include "nla.h"

//----------------------------------------------------------------------
class biguint_t : public BigNumStore<unsigned long>
{
public:
	biguint_t ( void );
	biguint_t ( const biguint_t &x );
	biguint_t& operator=(const biguint_t &x);

	biguint_t ( int, unsigned int c );
	biguint_t ( const unsigned long *b, unsigned int blen );

	biguint_t ( long x );
	biguint_t ( int x );
	biguint_t ( short x );

	biguint_t ( unsigned long x );
	biguint_t ( unsigned int x );
	biguint_t ( unsigned short x );

	~biguint_t();

	long           to_long   ( void ) const;
	int            to_int    ( void ) const;
	short          to_short  ( void ) const;

	unsigned long  to_ulong  ( void ) const;
	unsigned int   to_uint   ( void ) const;
	unsigned short to_ushort ( void ) const;

	int            compareTo  ( const biguint_t &x ) const;
	bool           operator== ( const biguint_t &x ) const;
	bool           operator!= ( const biguint_t &x ) const;
	bool           operator<  ( const biguint_t &x ) const;
	bool           operator<= ( const biguint_t &x ) const;
	bool           operator>= ( const biguint_t &x ) const;
	bool           operator>  ( const biguint_t &x ) const;

	biguint_t      operator+  ( const biguint_t &x ) const;
	biguint_t      operator-  ( const biguint_t &x ) const;
	biguint_t      operator*  ( const biguint_t &x ) const;
	biguint_t      operator/  ( const biguint_t &x ) const;
	biguint_t      operator%  ( const biguint_t &x ) const;
	biguint_t      operator&  ( const biguint_t &x ) const;
	biguint_t      operator|  ( const biguint_t &x ) const;
	biguint_t      operator^  ( const biguint_t &x ) const;
	biguint_t      operator<< ( int b ) const;
	biguint_t      operator>> ( int b ) const;

	void           operator+=  ( const biguint_t &x );
	void           operator-=  ( const biguint_t &x );
	void           operator*=  ( const biguint_t &x );
	void           operator/=  ( const biguint_t &x );
	void           operator%=  ( const biguint_t &x );
	void           operator&=  ( const biguint_t &x );
	void           operator|=  ( const biguint_t &x );
	void           operator^=  ( const biguint_t &x );
	void           operator<<= ( int b );
	void           operator>>= ( int b );

	void           operator++  ( void ); // prefix
	void           operator--  ( void );

	void           operator++  ( int ); // postfix
	void           operator--  ( int );

	void           add       ( const biguint_t &a, const biguint_t &b );
	void           subtract  ( const biguint_t &a, const biguint_t &b );
	void           multiply  ( const biguint_t &a, const biguint_t &b );
	void           divide    ( const biguint_t &b, biguint_t &q );

	unsigned int   bitLength ( void ) const;
	unsigned long  getBlock  ( unsigned int i ) const;
	void           setBlock  ( unsigned int i, unsigned long newBlock );
	bool           getBit    ( unsigned int bi ) const;
	void           setBit    ( unsigned int bi, bool newBit );
	void           trimLeft  ( void );

protected:

	void           bitwiseAnd ( const biguint_t &a, const biguint_t &b );
	void           bitwiseOr  ( const biguint_t &a, const biguint_t &b );
	void           bitwiseXor ( const biguint_t &a, const biguint_t &b );

	void           shiftLeft  ( const biguint_t &a, int b );
	void           shiftRight ( const biguint_t &a, int b );

	friend unsigned long getShiftedBlock(const biguint_t &num, unsigned int x, unsigned int y);

	template <class X>
	friend X convertBigUnsignedToPrimitiveAccess(const biguint_t &a);

	template <class X>
	X convertToSignedPrimitive(void) const;

	template <class X>
	X convertToPrimitive(void) const;
};

//----------------------------------------------------------------------
inline bool biguint_t::operator==(const biguint_t &x) const
{
	return BigNumStore<unsigned long>::operator==(x);
}

//----------------------------------------------------------------------
inline bool biguint_t::operator!=(const biguint_t &x) const
{
	return BigNumStore<unsigned long>::operator!=(x);
}

//----------------------------------------------------------------------
inline bool biguint_t::operator<(const biguint_t &x) const
{
	return compareTo(x) < 0;
}

//----------------------------------------------------------------------
inline bool biguint_t::operator<=(const biguint_t &x) const
{
	return compareTo(x) <= 0;
}

//----------------------------------------------------------------------
inline bool biguint_t::operator>=(const biguint_t &x) const
{
	return compareTo(x) >= 0;
}

//----------------------------------------------------------------------
inline bool biguint_t::operator>(const biguint_t &x) const
{
	return compareTo(x) > 0;
}

//----------------------------------------------------------------------
inline void biguint_t::operator+=(const biguint_t &x)
{
	add(*this, x);
}

//----------------------------------------------------------------------
inline void biguint_t::operator-=(const biguint_t &x)
{
	subtract(*this, x);
}

//----------------------------------------------------------------------
inline void biguint_t::operator*=(const biguint_t &x)
{
	multiply(*this, x);
}

//----------------------------------------------------------------------
inline void biguint_t::operator/=(const biguint_t &x)
{
	if (x.len==0) throw "biguint_t::operator /=: division by zero";
	biguint_t q;
	divide(x, q);
	*this = q;
}

//----------------------------------------------------------------------
inline void biguint_t::operator%=(const biguint_t &x)
{
	if (x.len==0) throw "biguint_t::operator %=: division by zero";
	biguint_t q;
	divide(x, q);
}

//----------------------------------------------------------------------
inline void biguint_t::operator&=(const biguint_t &x)
{
	bitwiseAnd(*this, x);
}

//----------------------------------------------------------------------
inline void biguint_t::operator|=(const biguint_t &x)
{
	bitwiseOr(*this, x);
}

//----------------------------------------------------------------------
inline void biguint_t::operator^=(const biguint_t &x)
{
	bitwiseXor(*this, x);
}

//----------------------------------------------------------------------
inline void biguint_t::operator<<=(int b)
{
	shiftLeft(*this, b);
}

//----------------------------------------------------------------------
inline void biguint_t::operator>>=(int b)
{
	shiftRight(*this, b);
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator+(const biguint_t &x) const
{
	biguint_t ans;
	ans.add(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator-(const biguint_t &x) const
{
	biguint_t ans;
	ans.subtract(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator*(const biguint_t &x) const
{
	biguint_t ans;
	ans.multiply(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator/(const biguint_t &x) const
{
	if (x.len==0) throw "biguint_t::operator /: division by zero";
	biguint_t q;
	biguint_t r = *this;
	r.divide(x, q);
	return q;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator%(const biguint_t &x) const
{
	if (x.len==0) throw "biguint_t::operator %: division by zero";
	biguint_t q, r;
	r = *this;
	r.divide(x, q);
	return r;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator&(const biguint_t &x) const
{
	biguint_t ans;
	ans.bitwiseAnd(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator|(const biguint_t &x) const
{
	biguint_t ans;
	ans.bitwiseOr(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator^(const biguint_t &x) const
{
	biguint_t ans;
	ans.bitwiseXor(*this, x);
	return ans;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator<<(int b) const
{
	biguint_t ans;
	ans.shiftLeft(*this, b);
	return ans;
}

//----------------------------------------------------------------------
inline biguint_t biguint_t::operator>>(int b) const
{
	biguint_t ans;
	ans.shiftRight(*this, b);
	return ans;
}

//----------------------------------------------------------------------
inline void biguint_t::operator++(int) { operator++(); }
inline void biguint_t::operator++(void)
{
	unsigned int i;
	bool carry = true;
	for (i=0;i<len&&carry;i++)
	{
		blk[i]++;
		carry = (blk[i] == 0);
	}
	if (carry)
	{
		// Allocate and then increase length, as in divide
		allocateAndCopy(len + 1);
		len++;
		blk[i] = 1;
	}
}

//----------------------------------------------------------------------
inline void biguint_t::operator--(int) { operator--(); }
inline void biguint_t::operator--(void)
{
	if (len == 0)
		throw "biguint_t::operator--(): Cannot decrement an unsigned zero";
	unsigned int i;
	bool borrow = true;
	for (i = 0; borrow; i++)
	{
		borrow = (blk[i] == 0);
		blk[i]--;
	}
	trimLeft();
}

//----------------------------------------------------------------------
inline unsigned long biguint_t::getBlock(unsigned int i) const
{
	return i >= len ? 0 : blk[i];
}

//----------------------------------------------------------------------
inline bool biguint_t::getBit(unsigned int bi) const
{
	return (getBlock(bi / N) & (((unsigned long)(1)) << (bi % N))) != 0;
}

#endif

