#include <Engine\Core\MemoryMgr.h>
#include <Engine\Util\Assert.h>
#include <Engine\Util\ConsolePrint.h>

#include <iostream>

#define ROUND_UP(addr,align) ((reinterpret_cast<uintptr_t>(addr) + (align - 1)) & -align)
#define ROUND_DOWN(addr,align) (reinterpret_cast<uintptr_t>(addr) & -align)

namespace Engine
{
	MemoryMgr* MemoryMgr::_instance = nullptr;

	void MemoryMgr::initialize(size_t memSize, unsigned int numDesc)
	{
		MessagedAssert(memSize > 0, "memSize should be positive.");
		MessagedAssert(numDesc>0, "numDesc should be positive.");

		this->memSize = memSize;
		this->numTotalDesc = numDesc;
		this->numUnusedDesc = numDesc;
		totalMemSize = memSize + numDesc*sizeof(MemBlockDesc);
		descBase = static_cast<uint8_t*>(malloc(totalMemSize));
		MessagedAssert(descBase != NULL, "Memory Manager not able to acquired memory.");
		DEBUG_PRINT("Memory Manager acquired memory - %d Bytes.\n", totalMemSize);

#if defined (_DEBUG)
		memset(descBase, noMansLandFill, totalMemSize);
#endif
		// set up the all the desc
		uint8_t* pos = descBase + (numDesc - 1)*sizeof(MemBlockDesc);
		MemBlockDesc* memDesc = reinterpret_cast<MemBlockDesc*>(pos);
		MemBlockDesc* nextDesc = NULL;

		for (unsigned int i = 0; i < numDesc-1; i++)
		{
			memDesc->next = nextDesc;
			memDesc->size = 0;
			nextDesc = memDesc;
			memDesc--;
		}
		
		unusedDescList = nextDesc;

		freeMemDescList = memDesc;
		freeMemDescList->next = NULL;
		freeMemDescList->base = descBase + numDesc*sizeof(MemBlockDesc);
		memoryBase = freeMemDescList->base;
		freeMemDescList->size = memSize;
		numBlocks++;
		numUnusedDesc--;
		MemBlockDesc** desc = reinterpret_cast<MemBlockDesc**>(freeMemDescList->base);
		*desc = freeMemDescList;

		allocMemDescList = NULL;
	}

	void MemoryMgr::CreateMemoryMgr()
	{
		if (_instance == NULL)
		{
			_instance = new MemoryMgr();
			_instance->initialize(1024 * 1024 * 2, 100);
		}
		else
		{
			DEBUG_PRINT("MemoryMgr already created !!\n");
		}
	}

	void MemoryMgr::DestroyMemoryMgr()
	{
		delete _instance;
		_instance = NULL;
	}

	MemoryMgr* MemoryMgr::getInstance()
	{
		return _instance;
	}

	MemoryMgr::MemoryMgr()
	{
		numBlocks = 0;
		DEBUG_PRINT("Creating MemoryMgr !!\n");
	}

	MemoryMgr::~MemoryMgr()
	{
		free(descBase);
		DEBUG_PRINT("Destroying MemoryMgr !!\n");
	}

	void* MemoryMgr::allocMemory( size_t reqSize )
	{
		return allocMemory( reqSize, MemoryAlignment(CACHE_LINE_ALIGNMENT_BYTES));
	}

	void* MemoryMgr::allocMemory(size_t reqSize, MemoryAlignment memoryAlignment)
	{
		// use first fit algorithm
		size_t totReqSize = reqSize + 2 * GAURD_SIZE + sizeof(MemBlockDesc*);
		MemBlockDesc* currentDesc = freeMemDescList;
		size_t alignmentDiff = 0;

		for (; currentDesc != NULL; currentDesc = currentDesc->next)
		{
			if (currentDesc->size >= totReqSize)
			{
				alignmentDiff = reinterpret_cast<size_t>(currentDesc->base + (currentDesc->size - totReqSize + GAURD_SIZE + sizeof(MemBlockDesc*)))
											- ROUND_DOWN(currentDesc->base + (currentDesc->size - totReqSize + GAURD_SIZE + sizeof(MemBlockDesc*)), memoryAlignment);

				if (alignmentDiff == 0)
				{
					break;
				}
				else if (currentDesc->size >= totReqSize + alignmentDiff)
				{
					totReqSize = totReqSize + alignmentDiff;
					break;
				}
			}
		}

		MessagedAssert(currentDesc != NULL, "Cannot allocate memory !!");
		if (currentDesc == NULL)
		{
			return NULL;
		}

		MemBlockDesc* freeDesc;

		// perfect fit
		if (totReqSize == currentDesc->size)
		{
			delDescFromList(freeMemDescList, currentDesc);
			addDescToList(allocMemDescList, currentDesc, false);

			freeDesc = currentDesc;
		}
		else
		{
			freeDesc = getUnusedDesc();

			if (freeDesc == NULL)
			{
				return NULL;
			}

			freeDesc->base = currentDesc->base + (currentDesc->size - totReqSize);
			freeDesc->size = totReqSize;
			currentDesc->size -= totReqSize;
			numBlocks++;

			addDescToList( allocMemDescList, freeDesc, false );
		}

		// || DESC || GAURD || DATA || GAURD ||
		MemBlockDesc** desc = reinterpret_cast<MemBlockDesc**>(freeDesc->base);
		*desc = freeDesc;

#if defined (_DEBUG)
		uint16_t* guard = reinterpret_cast<uint16_t*>(desc + 1);
		*guard = MEMORY_GAURD;

		uint8_t* fill = reinterpret_cast<uint8_t*>(guard + 1);
		memset(fill, cleanLandFill, reqSize);

		uint8_t* alignFill = fill+reqSize;
		memset(alignFill, alignLandFill, alignmentDiff);

		guard = reinterpret_cast<uint16_t*>(alignFill+alignmentDiff);
		*guard = MEMORY_GAURD;
#endif
		return freeDesc->base + GAURD_SIZE + sizeof(MemBlockDesc*);
	}

	void MemoryMgr::dellocMemory(void* ptr)
	{
		uint8_t* data = reinterpret_cast<uint8_t*>(ptr);
		MemBlockDesc** base = reinterpret_cast<MemBlockDesc**>(data - GAURD_SIZE - sizeof(MemBlockDesc**));
		MemBlockDesc* desc = *base;

		// find the previous node
		delDescFromList(allocMemDescList, desc);
		addDescToList(freeMemDescList, desc, true);

#if defined (_DEBUG)
		uint16_t* guardStart = reinterpret_cast<uint16_t*>(data - 2);
		MessagedAssert(*guardStart == MEMORY_GAURD, "Memory corrupted !!");

		size_t size = desc->size;
		uint16_t* guardEnd = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(base)+size - GAURD_SIZE);
		MessagedAssert(*guardEnd == MEMORY_GAURD, "Memory corrupted !!");
		memset(desc->base+sizeof(MemBlockDesc*), deadLandFill, size-sizeof(MemBlockDesc*));
#endif
	}

	void MemoryMgr::runGC()
	{
		MessagedAssert(freeMemDescList, "Free memory list currupted.");

		// combine the desc that come after the current desc
		MemBlockDesc* currentDesc = freeMemDescList;

		for (; currentDesc&&currentDesc->next;)
		{
			if (currentDesc->base + currentDesc->size == currentDesc->next->base)
			{
				MemBlockDesc* nextDesc = currentDesc->next;
				currentDesc->size += nextDesc->size;
				nextDesc->size = 0;
				
#if defined (_DEBUG)
				memset(nextDesc->base, deadLandFill, sizeof(MemBlockDesc*));
#endif

				delDescFromList(freeMemDescList, nextDesc);
				recycleDesc( nextDesc );
				numBlocks--;
			}
			else
			{
				currentDesc = currentDesc->next;
			}
		}
	}

	bool MemoryMgr::isValidMem( void* ptr )
	{
		uint8_t* base = reinterpret_cast<uint8_t*>(ptr);

		uint8_t* end = memoryBase+memSize;
		if (base >= memoryBase && base < end)
		{
			return true;
		}

		return false;
	}

	void MemoryMgr::delDescFromList(MemBlockDesc*& list, MemBlockDesc* node)
	{
		MessagedAssert( list, "List is NULL\n" );
		MessagedAssert( node, "Descriptor is Null\n" );

		MemBlockDesc* prevNode = NULL;
		MemBlockDesc* currentNode = list;

		for (; currentNode&&currentNode!=node; currentNode = currentNode->next)
		{
			prevNode = currentNode;
		}

		MessagedAssert(currentNode,"Descriptor not found.\n");

		if (prevNode)
		{
			prevNode->next = currentNode->next;
		}
		else
		{
			list = list->next;
		}
	}

	void MemoryMgr::addDescToList(MemBlockDesc*& list, MemBlockDesc* node, bool sortedInsert)
	{
		MessagedAssert(node, "Node is Null\n");

		MemBlockDesc* currentNode = list;
		MemBlockDesc* prevNode = NULL;

		if (sortedInsert)
		{
			for (; currentNode&&node->base > currentNode->base; currentNode = currentNode->next)
			{
				prevNode = currentNode;
			}

			MessagedAssert(node != currentNode, "Re-inserting the node into list.\n");

			node->next = currentNode;

			if (prevNode)
			{
				prevNode->next = node;
			}
			else
			{
				list = node;
			}
		}
		else
		{
			node->next = list;
			list = node;
		}
	}

	MemoryMgr::MemBlockDesc* MemoryMgr::getUnusedDesc()
	{
		MessagedAssert(numUnusedDesc>0, "No more memory descriptors left.");
		if (numUnusedDesc == 0)
		{
			return NULL;
		}

		MemBlockDesc* emptyDesc = unusedDescList;
		unusedDescList = unusedDescList->next;
		numUnusedDesc--;
		return emptyDesc;
	}

	void MemoryMgr::recycleDesc(MemBlockDesc* desc)
	{
		desc->next = unusedDescList;
		unusedDescList = desc;
		numUnusedDesc++;
	}

	size_t MemoryMgr::getAvailableMem()
	{
		size_t availableMem = 0;

		for (MemBlockDesc* node = freeMemDescList; node; node = node->next)
		{
			availableMem += node->size;
		}

		return availableMem;
	}
}