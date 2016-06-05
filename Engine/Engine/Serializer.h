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
			virtual void keyboardUpdate(unsigned int i_VKeyID, bool i_bDown);

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
