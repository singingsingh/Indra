#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <Engine\Core\WaitableObject.h>

namespace Engine
{
	class Mutex : public WaitableObject
	{
		public:
			Mutex( const char * i_pName );
			~Mutex();

			bool TryLock( void );
			void Lock( void );
			void Release( void );
	};

} // namespace Engine

#endif // __MUTEX_H__
