#include <Engine\Engine\GameObject.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Core\StringPool.h>
#include <Engine\Engine\Serializer.h>

namespace Engine
{
	DEFINE_MEMORY_MANAGER(GameObject);

	GameObject::GameObject(const char * name)
	{
		DEBUG_PRINT("Creating gameobject --  \"%s\"\n", name);

		_name = Engine::StringPool::Add(name);
		rotDegree = 0.0f;
		Serializer::RegisterSerialization( this );
	}

	GameObject::~GameObject()
	{
		DEBUG_PRINT("Destroying the GameObject %s.\n", _name);
	}

	const char* GameObject::getName() const
	{
		return _name;
	}

	uint8_t* GameObject::serialize(uint8_t* buffer)
	{
		long* bufferLong = reinterpret_cast<long *>(buffer);
		*bufferLong = UID;
		bufferLong++;

		float* bufferFloat = reinterpret_cast<float *>(bufferLong);
		*bufferFloat = pos.x;
		bufferFloat++;
		
		*bufferFloat = pos.y;
		bufferFloat++;

		*bufferFloat = rotDegree;
		bufferFloat++;

		uint8_t* bufferuint8 = reinterpret_cast<uint8_t*>(bufferFloat);
		return bufferuint8;
	}

	uint8_t* GameObject::deSerialize(void* buffer)
	{
		float* bufferFloat = reinterpret_cast<float *>(buffer);
		pos.x = *bufferFloat;
		bufferFloat++;

		pos.y = *bufferFloat;
		bufferFloat++;

		rotDegree = *bufferFloat;
		bufferFloat++;

		uint8_t* bufferuint8 = reinterpret_cast<uint8_t*>(bufferFloat);
		return bufferuint8;
	}

	void GameObject::collectSize(size_t & size)
	{
		size += sizeof(pos);
		size += sizeof(rotDegree);
		size += sizeof(UID);
	}
}