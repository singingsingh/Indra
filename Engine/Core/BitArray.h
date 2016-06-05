#ifndef __BITARRAY__
#define __BITARRAY__

#include <Engine\System\Target.h>

namespace Engine
{
	class BitArray
	{
		public:
			BitArray(unsigned int numBits, bool startClear = true);
			BitArray(const BitArray & rhs);
			BitArray operator=(const BitArray & rhs);
			~BitArray();

			inline void clear();
			inline void set();

			bool isEmpty();
			bool isFull();

			inline bool isBitSet(unsigned int bitNumber);
			inline bool isBitClear(unsigned int bitNumber);

			inline void setBit(unsigned int bitNumber);
			inline void clearBit(unsigned int bitNumber);

			bool getFirstClearBit(unsigned int & bitNumber);
			bool getFirstSetBit(unsigned int & bitNumber);

			BitArray operator~();
			BitArray operator&( const BitArray & rhs );
			BitArray operator|( const BitArray & rhs );
			BitArray operator^(const BitArray & rhs);
			bool operator[](unsigned int index);

		private:
			uint8_t* _actualBase;
			MYUINT* _base;
			size_t _bytesAllocated;
			size_t _bytesUsed;
			unsigned int _numBits;

			void _initialize( unsigned int numBits );
	};
}	// namespace Engine

#include <Engine\Core\BitArray.ini.h>

#endif	//__BITARRAY__
