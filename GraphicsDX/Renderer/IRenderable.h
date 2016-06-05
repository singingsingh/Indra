#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

#include <Engine\Engine\GameObject.h>
#include <Engine\Core\SmartPtr.h>

#include <string>

namespace GraphicsDX
{
	class IRenderable
	{
		public :

			virtual std::string getType() = 0;
			virtual void setGameObject(Engine::SmartPtr< Engine::GameObject >& i_gameObject) = 0;
			virtual Engine::SmartPtr< Engine::GameObject > getGameObject() = 0;

		protected:

			Engine::SmartPtr< Engine::GameObject > _gameObject;

	};	// class IRenderable
}	// namespace GraphicsDX

#endif	//__IRENDERABLE_H__
