#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <Engine\Core\WaitableObject.h>

namespace Engine
{
	class Semaphore : public WaitableObject
	{
		public:
			Semaphore( const char * i_pName, unsigned int i_CountMax, unsigned int i_CountInit );
			~Semaphore();

			void Increment( unsigned int i_count = 1 );
			void WaitAndDecrement( void );
	};

} // namespace Engine

#endif // __SEMAPHORE_H__