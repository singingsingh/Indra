#include <Engine\Core\FixedSizeAlloc.h>
#include <Engine\Util\ConsolePrint.h>

#include <stdlib.h>

namespace Engine
{
	FixedSizeAlloc* FixedSizeAlloc::instance_8 = NULL;

	FixedSizeAlloc::FixedSizeAlloc(unsigned int allocationSize, unsigned int numOfBlocks)
		:_allocationSize(allocationSize),
		_numOfBlocks(numOfBlocks),
		_totalSize(allocationSize*numOfBlocks),
		_allocatedSize(0),
		_allocatedBlocks(0)
	{
		_memoryBase = (uint8_t*)malloc(_totalSize);
		_freeBlock = const_cast<uint8_t*>(_memoryBase);

		// initialize the next pointers
		uint8_t* nextBlock = const_cast<uint8_t*>(_memoryBase) + _allocationSize;
		uint8_t* currentBlock = const_cast<uint8_t*>(_memoryBase);
		uint8_t** header = NULL;

		for (unsigned int i = 0; i < _numOfBlocks-1; i++)
		{
			header = reinterpret_cast<uint8_t**>(currentBlock);
			*header = nextBlock;

			currentBlock = nextBlock;
			nextBlock = nextBlock + _allocationSize;
		}

		header = reinterpret_cast<uint8_t**>(currentBlock);
		*header = NULL;
	}

	FixedSizeAlloc::~FixedSizeAlloc()
	{
		free(const_cast<uint8_t*>(_memoryBase));
	}

	void FixedSizeAlloc::createFixedSizeAllocater_8(unsigned int numOfBlocks)
	{
		if (instance_8 == NULL)
		{
			instance_8 = new FixedSizeAlloc(8, numOfBlocks);
		}
		else
		{
			DEBUG_PRINT("Fixed Size Allocator of size 8 already created.\n");
		}
	}

	FixedSizeAlloc & FixedSizeAlloc::getInstance_8()
	{
		return *instance_8;
	}

	void FixedSizeAlloc::destroyFixedSizeAllocator_8()
	{
		delete instance_8;
		instance_8 = NULL;
	}

	unsigned int FixedSizeAlloc::getAllocationSize()
	{
		return _allocationSize;
	}

	unsigned int FixedSizeAlloc::getNumFreeBlocks()
	{
		return _numOfBlocks - _allocatedBlocks;
	}

	unsigned int FixedSizeAlloc::getNumAllocBlocks()
	{
		return _allocatedBlocks;
	}

	unsigned int FixedSizeAlloc::getTotalSize()
	{
		return _totalSize;
	}

	unsigned int FixedSizeAlloc::getAllocatedSize()
	{
		return _allocatedSize;
	}

	unsigned int FixedSizeAlloc::getAvailableSize()
	{
		return _totalSize - _allocatedSize;
	}

	uint8_t* FixedSizeAlloc::allocMemory()
	{
		uint8_t* memory = _freeBlock;
		uint8_t** address = reinterpret_cast<uint8_t**>(_freeBlock);
		_freeBlock = *(address);
		
		_allocatedBlocks++;
		_allocatedSize += _allocationSize;

		return memory;
	}

	void FixedSizeAlloc::freeMemory(void* ptr)
	{
		uint8_t* memory = reinterpret_cast<uint8_t*>(ptr);
		uint8_t** address = reinterpret_cast<uint8_t**>(memory);
		*address = _freeBlock;
		_freeBlock = memory;

		_allocatedBlocks--;
		_allocatedSize -= _allocatedSize;
	}
}