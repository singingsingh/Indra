#ifndef __AI_UPDATE_NOTIFIER_H__
#define __AI_UPDATE_NOTIFIER_H__

#include <Engine\Engine\IAIUpdateListener.h>

#include <vector>

namespace Engine
{
	class AIUpdateNotifier
	{
		public:

			static void CreateAIUpdateNotifier();
			static void DestroyAIUpdateNotifier();
			static void UpdateAITick(double deltaTime);

			static void RegisterAIUpdate(IAIUpdateListener* aiListner);
			static void DeRegisterAIUpdate(IAIUpdateListener* aiListner);

		private:

			std::vector<IAIUpdateListener*> _list;
			static AIUpdateNotifier* _instance;

			AIUpdateNotifier();
			~AIUpdateNotifier();
	};
}

#endif	//__AI_UPDATE_NOTIFIER_H__
