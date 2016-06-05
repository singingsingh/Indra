#include <Engine\Core\StringPool.h>

namespace UnitTest
{
	void StringPoolUT()
	{
		Engine::StringPool::CreateStringPool(21);
		Engine::StringPool::CreateStringPool(10);

		Engine::StringPool::Add("vikram");
		Engine::StringPool::Add("vikram");
		Engine::StringPool::Add("singh");

		Engine::StringPool::DestroyStringPool();
	}
}