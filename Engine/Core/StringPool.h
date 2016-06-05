#ifndef __STRING_POOL_H__
#define __STRING_POOL_H__

#include <stdint.h>

namespace Engine
{
	class StringPool
	{
		public:

			static void CreateStringPool(size_t size);
			static void DestroyStringPool();

			static const char * Add( const char* string );
			static const char * Find( const char* string );

		private :

			static StringPool* _instance;
			const uint8_t* poolStart, *poolEnd;
			uint8_t* currentPos;

			StringPool( size_t size );
			~StringPool();

			const char * _add( const char* string );
			const char * _find( const char * string );
	};
}

#endif	//__STRING_POOL_H__