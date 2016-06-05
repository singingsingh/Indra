#ifndef __I_SERIALIZABLE_H__
#define __I_SERIALIZABLE_H__

#include <stdint.h>

namespace Engine
{
	class ISerializable
	{
		public:
			virtual uint8_t* serialize(uint8_t* buffer) = 0;
			virtual uint8_t* deSerialize(void* buffer) = 0;
			virtual void collectSize(size_t& size) = 0;
	};
}	// namespace Engine

#endif	//__I_SERIALIZABLE_H__
