#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

#include <Engine\Util\Assert.h>

namespace Engine
{
	template <class T>
	class SmartPtr
	{
		public :

			inline SmartPtr();
			inline SmartPtr(T* i_ptr);
			inline SmartPtr(const SmartPtr & i_rhs);
			inline ~SmartPtr();

			inline T & operator*();
			inline T * operator->();
			inline bool operator==(const SmartPtr& i_rhs);
			inline bool operator!=(const SmartPtr& i_rhs);
			inline SmartPtr & operator=(const SmartPtr & i_rhs);

		private:

			T* _ptr;
			unsigned short* _refCount;

			inline void releaseRef();
			inline void acquireRef(const SmartPtr& i_other);
	};
}

#include <Engine\Core\SmartPtr.ini.h>

#endif	//__SMART_PTR_H__