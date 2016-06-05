#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <Engine\Core\Math\Vector2f.h>
#include <Engine\Core\MemoryMgr.h>
#include <Engine\Engine\ISerializable.h>

namespace Engine
{
	class GameObject : public ISerializable
	{
		public :
			GameObject(const char * name);
			~GameObject();

			const char * getName() const;
			Math::Vector2f pos;
			float rotDegree;
			long UID;

			virtual uint8_t* serialize(uint8_t* buffer);
			virtual uint8_t* deSerialize(void* buffer);
			virtual void collectSize(size_t& size);

		private :
			
			const char * _name;
			
			GameObject(const GameObject & rhs);
			GameObject& operator=(const GameObject & rhs);

			DECLARE_MEMORY_MANAGER;
	};
}

#endif	//_GAME_OBJECT_H_