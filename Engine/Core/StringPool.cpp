#include <Engine\Core\StringPool.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Util\Assert.h>

#include <iostream>

namespace Engine
{
	Engine::StringPool* StringPool::_instance = nullptr;

	void StringPool::CreateStringPool( size_t i_size )
	{
		if (_instance == nullptr)
		{
			_instance = new Engine::StringPool(i_size);
		}
		else
		{
			MessagedAssert(false,  "Cannot create two instance of StringPool class.\n");
		}
	}

	void StringPool::DestroyStringPool()
	{
		delete _instance;
		_instance = nullptr;
	}

	StringPool::StringPool( size_t i_size )
		:poolStart ((uint8_t*)malloc(i_size)),
		poolEnd(poolStart + i_size)
	{
		DEBUG_PRINT("Creating String Pool.\n");

		
		currentPos = const_cast<uint8_t*>(poolStart);
	}

	StringPool::~StringPool()
	{
		DEBUG_PRINT("Destroying String Pool.\n");
		free(const_cast<uint8_t*>(poolStart));
		currentPos = nullptr;
	}

	const char * StringPool::_add(const char * i_string)
	{
		const char * location = _find(i_string);
		
		if (location == nullptr)
		{
			size_t length = strlen(i_string)+1;
			MessagedAssert(currentPos+sizeof(size_t)+length <= poolEnd, "StringPool ran out of memory.\n" );

			size_t* tempPos = reinterpret_cast<size_t*>(currentPos);
			*tempPos = length;
			tempPos += 1;

			char* stringPos = reinterpret_cast<char*>(tempPos);
			strcpy_s( stringPos, length, i_string );

			currentPos = reinterpret_cast<uint8_t*>(stringPos)+ length;
			return stringPos;
		}
		else
		{
			return location;
		}
	}

	const char * Engine::StringPool::_find(const char * i_string)
	{
		uint8_t* tempPos = const_cast<uint8_t*>(poolStart);
		size_t length = strlen(i_string)+1;

		while (tempPos < currentPos)
		{
			size_t currentLength = *reinterpret_cast<size_t*>(tempPos);

			if (length == currentLength)
			{
				char* localString = reinterpret_cast<char*>(tempPos + sizeof(size_t));

				if (strcmp(i_string, localString) == 0)
				{
					return localString;
				}
			}

			tempPos += currentLength + sizeof(size_t);
		}

		return nullptr;
	}

	const char * StringPool::Add(const char * i_string)
	{
		return _instance->_add(i_string);
	}

	const char * StringPool::Find(const char * i_string)
	{
		return _instance->_find(i_string);
	}
}