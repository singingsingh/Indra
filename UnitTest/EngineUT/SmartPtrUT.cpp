#include <Engine\Core\SmartPtr.h>

#include <iostream>

namespace UnitTest
{
	void SmartPtrUT()
	{
		Engine::SmartPtr<int> nullPtr;

		{
			int* i = new int(5);
			Engine::SmartPtr<int> ptr( i );
			Engine::SmartPtr<int> ptrAnother = ptr;
		}

		int* j = new int(10);
		Engine::SmartPtr<int> ptr2(j);
		{
			Engine::SmartPtr<int> ptr3 = nullPtr;
			ptr3 = ptr2;
		}

		struct Node
		{
			int index;
			char ch;
		};

		Engine::SmartPtr<Node> nodePtr( new Node() );
		nodePtr->ch = 'a';
		nodePtr->index = 3;

		int* k = new int(8);
		Engine::SmartPtr<int> equalPtr1(k);
		Engine::SmartPtr<int> equalPtr2 = equalPtr1;

		if (equalPtr1 == equalPtr2)
		{
			std::cout << "Both are equal.\n";
		}

		Engine::SmartPtr<int> equalPtr3;
		if (equalPtr1 != equalPtr3)
		{
			std::cout << "Both are not equal.\n";
		}

		Engine::SmartPtr<int> nullPtr2(nullptr);
	}
}