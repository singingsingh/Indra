#include <Engine\Core\BitArray.h>

#include <stdlib.h>
#include <string.h>

#define ROUND_UP(addr,align) ((reinterpret_cast<uintptr_t>(addr) + (align - 1)) & -align)

namespace Engine
{
	BitArray::BitArray(unsigned int numBits, bool startClear)
	{
		_initialize( numBits );

		int initializationVal = startClear ? 0 : ~0;
		memset(_base, initializationVal, _bytesUsed);
	}

	BitArray::BitArray(const BitArray & rhs)
	{
		_initialize(rhs._numBits);
		memcpy(this->_base, rhs._base, rhs._bytesUsed);
	}

	BitArray BitArray::operator=(const BitArray & rhs)
	{
		if (this == &rhs)
			return *this;

		free(this->_actualBase);

		_initialize(rhs._numBits);
		memcpy(this->_base, rhs._base, rhs._bytesUsed);
		return *this;
	}

	void BitArray::_initialize(unsigned int numBits)
	{
		size_t bytesNeeded = (numBits + (8 - 1)) / 8;
		bytesNeeded = ((bytesNeeded + (sizeof(MYUINT) - 1)) / sizeof(MYUINT))*sizeof(MYUINT);
		_bytesUsed = bytesNeeded;
		_actualBase = (uint8_t *)malloc(bytesNeeded);
		_base = reinterpret_cast<MYUINT*>(_actualBase);

		size_t alignmentDiff = ROUND_UP(_actualBase, CACHE_LINE_ALIGNMENT_BYTES) - reinterpret_cast<size_t>(_actualBase);
		if (alignmentDiff != 0)
		{
			bytesNeeded += alignmentDiff;
			_actualBase = (uint8_t*)realloc(_actualBase, bytesNeeded);
			uint8_t* base = reinterpret_cast<uint8_t*>(_actualBase);
			_base = reinterpret_cast<MYUINT*>(base + alignmentDiff);
		}

		_numBits = numBits;
		_bytesAllocated = bytesNeeded;
	}

	BitArray::~BitArray()
	{
		free(_actualBase);
		_actualBase = NULL;
	}

	bool BitArray::getFirstClearBit(unsigned int & bitNumber)
	{
		MYUINT allSet = ~0;
		size_t word = 0;
		size_t numWords = _bytesUsed / sizeof(MYUINT);

		while ((_base[word] == allSet) && (word < numWords))
			word++;

		if (word == numWords)
			return false;

		unsigned int bit = 0;
		MYUINT bitMask = 1;
		bitMask <<= sizeof(MYUINT) * 8 - 1;

		while ((_base[word] & bitMask) != 0)
		{
			bitMask >>= 1;
			bit++;
		}

		unsigned int bitNum = unsigned int(word * sizeof(MYUINT) * 8) + bit;

		if (bitNum < _numBits)
		{
			bitNumber = bitNum;
			return true;
		}

		return false;
	}

	bool BitArray::getFirstSetBit(unsigned int & bitNumber)
	{
		MYUINT allClear = 0;
		size_t word = 0;
		size_t numWords = _bytesUsed / sizeof(MYUINT);

		while ((_base[word] == allClear) && (word < numWords))
			word++;

		if (word == numWords)
			return false;

		unsigned int bit = 0;
		MYUINT bitMask = 1;
		bitMask <<= sizeof(MYUINT) * 8 - 1;

		while ((_base[word] & bitMask) == 0)
		{
			bitMask >>= 1;
			bit++;
		}

		unsigned int bitNum = unsigned int(word * sizeof(MYUINT) * 8) + bit;

		if (bitNum < _numBits)
		{
			bitNumber = bitNum;
			return true;
		}

		return false;
	}

	BitArray BitArray::operator~()
	{
		BitArray temp(this->_numBits);
		memcpy(temp._base, this->_base, this->_bytesUsed);
		
		size_t numWords = _bytesUsed / sizeof(MYUINT);
		for (unsigned int i = 0; i < numWords; i++)
		{
			temp._base[i] = ~(temp._base[i]);
		}

		return temp;
	}

	BitArray BitArray::operator&(const BitArray & rhs)
	{
		BitArray temp(this->_numBits);

		size_t numWords = _bytesUsed / sizeof(MYUINT);
		for (unsigned int i = 0; i < numWords; i++)
		{
			temp._base[i] = this->_base[i] & rhs._base[i];
		}

		return temp;
	}

	BitArray BitArray::operator|(const BitArray & rhs)
	{
		BitArray temp(this->_numBits);

		size_t numWords = _bytesUsed / sizeof(MYUINT);
		for (unsigned int i = 0; i < numWords; i++)
		{
			temp._base[i] = this->_base[i] | rhs._base[i];
		}

		return temp;
	}

	BitArray BitArray::operator^(const BitArray & rhs)
	{
		BitArray temp(this->_numBits);

		size_t numWords = _bytesUsed / sizeof(MYUINT);
		for (unsigned int i = 0; i < numWords; i++)
		{
			temp._base[i] = this->_base[i] ^ rhs._base[i];
		}

		return temp;
	}

	bool BitArray::operator[](unsigned int index)
	{
		return isBitSet(index);
	}

	bool BitArray::isEmpty(void)
	{
		unsigned int dummyBit;
		return getFirstSetBit(dummyBit) == false;
	}

	bool BitArray::isFull(void)
	{
		unsigned int dummyBit;
		return getFirstClearBit(dummyBit) == false;
	}
}