#ifndef __FIXED_SIZE_ALLOC__
#define __FIXED_SIZE_ALLOC__

#include <stdint.h>

#define DECLARE_FIXEDSIZE_ALLOCATOR_8									\
    public:																\
        void * operator new(size_t requestedSize);						\
        void operator delete(void * p, size_t requestedSize);			\
	public:																\
		static size_t size

#define INLINE_DECLARE_FIXEDSIZE_ALLOCATOR_8							\
    public:																\
        inline void * operator new(size_t requestedSize);				\
        inline void operator delete(void * p, size_t requestedSize);	\
	public:																\
		static size_t size

#define INLINE_DATA_DEFINE_FIXEDSIZE_ALLOCATOR_8(className)				\
		size_t className::size = 0

#define INLINE_FUNCTION_DEFINE_FIXEDSIZE_ALLOCATOR_8(className)				\
    void* className::operator new(size_t requestedSize)						\
	{																		\
		size += requestedSize;												\
        return Engine::FixedSizeAlloc::getInstance_8().allocMemory();		\
    }																		\
    void className::operator delete(void * ptr, size_t requestedSize)		\
	{																		\
        Engine::FixedSizeAlloc::getInstance_8().freeMemory(ptr);			\
		size -= requestedSize;												\
    }

#define DEFINE_FIXEDSIZE_ALLOCATOR_8(className)								\
	size_t className::size = 0;										\
    void* className::operator new(size_t requestedSize)						\
	{																		\
		size += requestedSize;												\
        return Engine::FixedSizeAlloc::getInstance_8().allocMemory();		\
    }																		\
    void className::operator delete(void * ptr, size_t requestedSize)		\
	{																		\
        Engine::FixedSizeAlloc::getInstance_8().freeMemory(ptr);			\
		size -= requestedSize;												\
    }

namespace Engine
{
	class FixedSizeAlloc
	{
		public :
			static void createFixedSizeAllocater_8( unsigned int numOfBlocks);
			static FixedSizeAlloc& getInstance_8();
			static void destroyFixedSizeAllocator_8();

			unsigned int getAllocationSize();
			unsigned int getNumFreeBlocks();
			unsigned int getNumAllocBlocks();
			unsigned int getTotalSize();
			unsigned int getAllocatedSize();
			unsigned int getAvailableSize();

			uint8_t* allocMemory();
			void freeMemory(void* ptr);

		private :

			static FixedSizeAlloc* instance_8;

			unsigned int _allocatedSize;
			unsigned int _allocatedBlocks;

			const unsigned int _allocationSize;
			const unsigned int _numOfBlocks;
			const unsigned int _totalSize;

			const uint8_t* _memoryBase;
			uint8_t* _freeBlock;

			FixedSizeAlloc(unsigned int allocationSize, unsigned int numOfBlocks);
			~FixedSizeAlloc();
			FixedSizeAlloc( const FixedSizeAlloc & rhs );
			FixedSizeAlloc& operator= ( const FixedSizeAlloc & rhs );
	};
}

#endif	//__FIXED_SIZE_ALLOC__