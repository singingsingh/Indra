#ifndef __I_AI_UPDATE_LISTENER_H__
#define __I_AI_UPDATE_LISTENER_H__

namespace Engine
{
	class IAIUpdateListener
	{
		public:
			virtual void aiUpdate(double deltaTime) = 0;
	};
}

#endif	//__I_AI_UPDATE_LISTENER_H__
