#include <Engine\Core\SmartPtr.h>
#include <Engine\Util\ConsolePrint.h>

namespace Engine
{
	template<class T>
	SmartPtr<T>::SmartPtr(T* i_ptr)
	{
		_ptr = i_ptr;

		if (_ptr == nullptr)
		{
			_refCount = nullptr;
		}
		else
		{
			_refCount = new unsigned short(1);
		}
	}

	template<class T>
	SmartPtr<T>::SmartPtr(const SmartPtr & i_rhs)
	{
		acquireRef(i_rhs);
	}

	template<class T>
	void SmartPtr<T>::acquireRef(const SmartPtr& i_other)
	{
		_ptr = i_other._ptr;
		_refCount = i_other._refCount;


		if (_ptr != nullptr)
		{
			(*_refCount)++;
		}
	}

	template<class T>
	T & SmartPtr<T>::operator * ()
	{
		MessagedAssert(_ptr != nullptr, "Indirection on nullptr.\n");
		return *_ptr;
	}

	template<class T>
	T * SmartPtr<T>::operator -> ()
	{
		MessagedAssert(_ptr != nullptr, "Access on nullptr.\n");
		return _ptr;
	}

	template<class T>
	inline bool SmartPtr<T>::operator==(const SmartPtr & i_rhs)
	{
		if (_ptr == i_rhs._ptr)
		{
			return true;
		}

		return false;
	}

	template<class T>
	inline bool SmartPtr<T>::operator!=(const SmartPtr & i_rhs)
	{
		if (_ptr != i_rhs._ptr)
		{
			return true;
		}

		return false;
	}

	template<class T>
	SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr& i_rhs)
	{
		// check for self reference
		if (_ptr == i_rhs._ptr)
		{
			return *this;
		}

		releaseRef();
		acquireRef(i_rhs);

		return *this;
	}

	template<class T>
	void SmartPtr<T>::releaseRef()
	{
		if (_ptr == nullptr)
		{
			return;
		}

		(*_refCount)--;

		if (*_refCount == 0)
		{
			DEBUG_PRINT("refCount == 0. Calling destructor.\n");
			delete _ptr;
			_ptr = nullptr;
			delete _refCount;
			_refCount = nullptr;
		}
	}

	template<class T>
	SmartPtr<T>::~SmartPtr()
	{
		releaseRef();
	}

	template<class T>
	SmartPtr<T>::SmartPtr()
		:_ptr(nullptr),
		_refCount(nullptr)
	{

	}
}