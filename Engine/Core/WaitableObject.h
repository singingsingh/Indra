#ifndef __WAITABLE_OBJECT_H__
#define __WAITABLE_OBJECT_H__

#include <Windows.h>

namespace Engine
{
	class WaitableObject
	{
		public:
			WaitableObject() :
				m_Handle( INVALID_HANDLE_VALUE )
			{
			}

			HANDLE GetHandle( void ) { return m_Handle; }

		protected:
			HANDLE m_Handle;
	};

} // namespace Engine

#endif // __WAITABLE_OBJECT_H__