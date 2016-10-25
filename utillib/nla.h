#ifndef _BIGNUM_STORE_H_
#define _BIGNUM_STORE_H_

//-------------------------------------------------------------------------------
template <class BaseType>
class BigNumStore
{
public:
	static const unsigned int N;
	unsigned int capacity;
	unsigned int len;
	BaseType *blk;

	                       BigNumStore ( void );
	                       BigNumStore ( const BigNumStore<BaseType>& x );
	BigNumStore<BaseType>& operator=   ( const BigNumStore<BaseType>& x );

	                       BigNumStore ( unsigned int size );
	                       BigNumStore ( const BaseType *b, unsigned int len );
                              ~BigNumStore();

	void allocate        ( unsigned int size );
	void allocateAndCopy ( unsigned int size );

	bool operator==      (const BigNumStore<BaseType>& x) const;
	bool operator!=      (const BigNumStore<BaseType>& x) const { return !operator==(x); }
};

//-------------------------------------------------------------------------------
template <class BaseType>
const unsigned int BigNumStore<BaseType>::N = 8 * sizeof(BaseType);

//-------------------------------------------------------------------------------
template <class BaseType>
inline BigNumStore<BaseType>::BigNumStore(void)
: capacity(0), len(0)
{
	blk = 0;
}

//-------------------------------------------------------------------------------
template <class BaseType>
inline BigNumStore<BaseType>::BigNumStore(unsigned int c)
: capacity(c), len(0)
{
	blk = (capacity > 0) ? (new BaseType[capacity]) : 0;
}

//-------------------------------------------------------------------------------
template <class BaseType>
BigNumStore<BaseType>& BigNumStore<BaseType>::operator=(const BigNumStore<BaseType> &x)
{
	if (this == &x)
		return *this;

	len = x.len;
	allocate(len);
	for (unsigned int i = 0; i < len; i++)
		blk[i] = x.blk[i];
	return *this;
}

//-------------------------------------------------------------------------------
template <class BaseType>
BigNumStore<BaseType>::BigNumStore(const BigNumStore<BaseType> &x)
: len(x.len)
{
	capacity = len;
	blk = new BaseType[capacity];
	for (unsigned int i = 0; i < len; i++)
		blk[i] = x.blk[i];
}

//-------------------------------------------------------------------------------
template <class BaseType>
BigNumStore<BaseType>::BigNumStore(const BaseType *b, unsigned int len)
: capacity(len), len(len)
{
#if 1
	blk = new BaseType[capacity];
	for (unsigned int i = 0; i < len; i++)
		blk[i] = b[i];
#else
	blk = new BaseType[capacity];
	for (unsigned int i = 0; i < len; i++)
		blk[i] = (b ? b[i] : 0);
#endif
}

//-------------------------------------------------------------------------------
template <class BaseType>
inline BigNumStore<BaseType>::~BigNumStore()
{
	if (blk)
		delete [] blk;
}

//-------------------------------------------------------------------------------
template <class BaseType>
void BigNumStore<BaseType>::allocate(unsigned int size)
{
	if (size > capacity)
	{
		if (blk)
			delete [] blk;
		capacity = size;
		blk = new BaseType[capacity];
	}
}

//-------------------------------------------------------------------------------
template <class BaseType>
void BigNumStore<BaseType>::allocateAndCopy(unsigned int size)
{
	if (size > capacity)
	{
		BaseType *oldBlk = blk;
		capacity = size;
		blk = new BaseType[capacity];
		for (unsigned int i = 0; i < len; i++)
			blk[i] = oldBlk[i];
		delete [] oldBlk;
	}
}

//-------------------------------------------------------------------------------
template <class BaseType>
bool BigNumStore<BaseType>::operator==(const BigNumStore<BaseType> &x) const
{
	if (len != x.len)
	{
		return false;

	} else
	{
		for (unsigned int i = 0; i < len; i++)
			if (blk[i] != x.blk[i])
				return false;
		return true;
	}
}

//-------------------------------------------------------------------------------
#define thisIsMe(a) (this==&a)

#endif
