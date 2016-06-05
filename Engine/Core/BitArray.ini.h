#include <Engine\Util\Assert.h>

#include <string.h>

namespace Engine
{
	bool BitArray::isBitSet(unsigned int bitNumber)
	{
		MessagedAssert( bitNumber<_numBits, "Invalid bit." );
		unsigned int wordSize = sizeof(MYUINT) * 8;
		unsigned int word = bitNumber / wordSize;
		unsigned int bit = wordSize - (bitNumber % wordSize) - 1;

		return (_base[word] & MYUINT(1 << bit)) != 0;
	}

	bool BitArray::isBitClear(unsigned int bitNumber)
	{
		MessagedAssert(bitNumber<_numBits, "Invalid bit.");

		return !isBitSet(bitNumber);
	}

	void BitArray::setBit(unsigned int bitNumber)
	{
		MessagedAssert(bitNumber<_numBits, "Invalid bit.");

		unsigned int wordSize = sizeof(MYUINT) * 8;
		unsigned int word = bitNumber / wordSize;
		unsigned int bit = wordSize - (bitNumber % wordSize) - 1;

		_base[word] |= MYUINT(1 << bit);
	}

	void BitArray::clearBit(unsigned int bitNumber)
	{
		MessagedAssert(bitNumber<_numBits, "Invalid bit.");

		unsigned int wordSize = sizeof(MYUINT) * 8;
		unsigned int word = bitNumber / wordSize;
		unsigned int bit = wordSize - (bitNumber % wordSize) - 1;

		_base[word] &= ~(1 << bit);
	}

	void BitArray::clear()
	{
		memset(_base, 0, _bytesUsed);
	}

	void BitArray::set()
	{
		memset(_base, ~0, _bytesUsed);
	}
}	// namespace Engine