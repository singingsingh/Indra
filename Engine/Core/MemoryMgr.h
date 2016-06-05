#ifndef __HEAP_MANAGER_H__
#define __HEAP_MANAGER_H__

#include <stdint.h>

#define DECLARE_MEMORY_MANAGER											\
    public:																\
        void * operator new(size_t requestedSize);						\
        void operator delete(void * p, size_t requestedSize);			\
		static size_t size

#define DEFINE_MEMORY_MANAGER(className)									\
	size_t className::size = 0;												\
    void* className::operator new(size_t requestedSize)						\
	{																		\
		size += requestedSize;												\
        return Engine::MemoryMgr::getInstance()->allocMemory(requestedSize);	\
    }																		\
    void className::operator delete(void * ptr, size_t requestedSize)		\
	{																		\
        Engine::MemoryMgr::getInstance()->dellocMemory(ptr);					\
		size -= requestedSize;												\
    }

namespace Engine
{
	enum MemoryAlignment { 
		ALIGHMENT_1_BYTES = 1, 
		ALIGHMENT_2_BYTES = 2, 
		ALIGHMENT_4_BYTES = 4, 
		ALIGHMENT_8_BYTES = 8, 
		ALIGHMENT_16_BYTES = 16,
		ALIGHMENT_32_BYTES = 32,
		ALIGHMENT_64_BYTES = 64,
		ALIGHMENT_128_BYTES = 128
	};

	class MemoryMgr
	{
	public:

		void initialize(size_t memSize, unsigned int numDesc);
		static void CreateMemoryMgr();
		static void DestroyMemoryMgr();
		static MemoryMgr* getInstance();
		void* allocMemory( size_t size );
		void* allocMemory( size_t size, MemoryAlignment memoryAlignment);
		void dellocMemory(void *);
		void runGC();
		size_t getAvailableMem();

	private:

		typedef struct MemBlockDescriptor
		{
			size_t size;
			uint8_t* base;
			struct MemBlockDescriptor * next;
		} MemBlockDesc;

#if defined (_DEBUG)
		static const unsigned char noMansLandFill = 0xFD;
		static const unsigned char alignLandFill = 0xED;
		static const unsigned char deadLandFill = 0xDD;
		static const unsigned char cleanLandFill = 0xCD;
#define MEMORY_GAURD 0xDEAD
#define GAURD_SIZE 2
#else 
#define GAURD_SIZE 0
#endif

		static MemoryMgr* _instance;

		uint8_t* descBase;
		uint8_t* memoryBase;
		size_t memSize, totalMemSize;
		MemBlockDesc* freeMemDescList;
		MemBlockDesc* allocMemDescList;
		MemBlockDesc* unusedDescList;
		unsigned int numBlocks;
		unsigned int numTotalDesc;
		unsigned int numUnusedDesc;

		MemoryMgr();
		~MemoryMgr();
		MemoryMgr(const MemoryMgr & rhs);
		MemoryMgr& operator=(const MemoryMgr & rhs);

		bool isValidMem( void* ptr );
		void delDescFromList( MemBlockDesc*& list, MemBlockDesc* node );
		void addDescToList( MemBlockDesc*& list, MemBlockDesc* node, bool sortedInsert );
		MemBlockDesc* getUnusedDesc();
		void recycleDesc( MemBlockDesc* desc );
	};
}

#endif	__HEAP_MANAGER_H__