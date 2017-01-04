#include <Engine\Engine\Serializer.h>
#include <Engine\Core\StringPool.h>
#include <Engine\Util\FileUtils.h>
#include <Engine\Util\Assert.h>
#include <Engine\Core\MemoryMgr.h>
#include <Engine\Engine\KeyboardNotifier.h>
#include <Engine\Util\ConsolePrint.h>
#include <Engine\Engine\World.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Engine
{
	Serializer* Serializer::_instance = nullptr;

	void Serializer::CreateSerializer(const char* i_savaFileName)
	{
		if (_instance == nullptr)
		{
			_instance = new Serializer(i_savaFileName);
		}
	}

	void Serializer::DestroySerializer()
	{
		delete _instance;
		_instance = nullptr;
	}

	void Serializer::RegisterSerialization(Engine::ISerializable* i_iSerializable)
	{
		MessagedAssert(i_iSerializable !=nullptr, "Null ptr passed for serialization.\n");

		_instance->_list.push_back( i_iSerializable );
	}

	void Serializer::keyboardUpdate(uint8_t i_key, bool i_down, uint16_t i_x, uint16_t i_y)
	{
		//switch (i_key)
		//{
		//case 0x4B:	// k
		//	if (i_down)
		//	{
		//		serializeObjects();
		//	}
		//	break;

		//case 0x4C:	// l
		//	if (i_down)
		//	{
		//		deSerializeObjects();
		//	}
		//	break;
		//}
	}

	void Serializer::mouseClickUpdate(uint8_t i_button, bool i_down, uint16_t i_x, uint16_t i_y)
	{
	}

	void Serializer::mouseMoveUpdate(bool i_leftBt, bool i_rightBt, bool i_middleBt, uint16_t i_x, uint16_t i_y)
	{
	}

	void Serializer::mousePassiveMoveUpdate(uint16_t i_x, uint16_t i_y)
	{
	}

	void Serializer::mouseWheelUpdate(bool i_direction, uint16_t i_x, uint16_t i_y)
	{
	}

	Serializer::Serializer(const char* i_savaFileName)
	{
		_saveFileName = Engine::StringPool::Add(i_savaFileName);
		Engine::KeyboardNotifier::RegisterKeyboardUpdate(this);
	}

	Serializer::~Serializer()
	{
		Engine::KeyboardNotifier::DeRegisterKeyboardUpdate(this);
		_list.clear();
	}

	void Serializer::serializeObjects()
	{
		std::vector < Engine::ISerializable* >::iterator itr = _list.begin();
		size_t size = 0;
		for (; itr != _list.end(); itr++)
		{
			(*itr)->collectSize(size);
		}

		MemoryMgr* memoryMgr = MemoryMgr::getInstance();
		uint8_t* currentPos = (uint8_t*)memoryMgr->allocMemory(size);
		uint8_t* start = currentPos;

		itr = _list.begin();
		for (; itr != _list.end(); itr++)
		{
			currentPos = (*itr)->serialize(currentPos);
		}

		// write it to the save file
		FILE * pFile = NULL;
		fopen_s(&pFile, _saveFileName, "wb");
		fwrite(start, size, 1, pFile);
		fclose(pFile);

		memoryMgr->dellocMemory(start);
	}

	void Serializer::deSerializeObjects()
	{
		size_t fileSize = 0;
		uint8_t* currentPos = (uint8_t*) Engine::FileUtils::LoadFile(_saveFileName, fileSize);
		uint8_t* start = currentPos;
		long currentUID = 0;
		SmartPtr< GameObject > curentGO;
		World* world = World::GetWorld();

		std::vector < Engine::ISerializable* >::iterator itr = _list.begin();
		for (; itr != _list.end(); itr++)
		{
			long* bufferLong = reinterpret_cast<long *>(currentPos);
			currentUID = *bufferLong;
			bufferLong++;

			curentGO = world->getGameObjectByUID( currentUID );
			currentPos = curentGO->deSerialize(bufferLong);
		}

		free(start);
	}
}