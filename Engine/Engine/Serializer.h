#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include <Engine\Engine\ISerializable.h>
#include <Engine\Engine\IKeyboardListener.h>

#include <vector>

namespace Engine
{
	class Serializer : public IKeyboardListener
	{
		public :
			static void CreateSerializer(const char* savaFileName);
			static void DestroySerializer();

			static void RegisterSerialization(Engine::ISerializable* iSerializable );
			
			virtual void keyboardUpdate(uint8_t key, bool down, uint16_t x, uint16_t y);
			virtual void mouseClickUpdate(uint8_t button, bool down, uint16_t x, uint16_t y);
			virtual void mouseMoveUpdate(bool leftBt, bool rightBt, bool middleBt, uint16_t x, uint16_t y);
			virtual void mousePassiveMoveUpdate(uint16_t x, uint16_t y);
			virtual void mouseWheelUpdate(bool direction, uint16_t x, uint16_t y);

		private:
			Serializer(const char* savaFileName);
			~Serializer();

			void serializeObjects();
			void deSerializeObjects();

			std::vector< Engine::ISerializable* > _list;

			static Serializer* _instance;
			const char* _saveFileName;
	};
}	// namespace Engine

#endif	//__SERIALIZER_H__
