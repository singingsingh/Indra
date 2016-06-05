#include <Engine\Core\Math\Vector4.h>

#include <iostream>

namespace UnitTest
{
	void Vector4UT()
	{
		std::cout << "Vector4 Unit Test" << std::endl;

		Engine::Math::Vector4 vec1( 1.0, 2.0, 3.0, 4.0 );
		Engine::Math::Vector4 vec2(3.0, 1.0, 4.0, 2.0);

		Engine::Math::Vector4 vec3;

		std::cout << "Vector 1 = " << vec1.x() << " " << vec1.y() << " " << vec1.z() << " " << vec1.w() << std::endl;
		std::cout << "Vector 2 = " << vec2.x() << " " << vec2.y() << " " << vec2.z() << " " << vec2.w() << std::endl;

		vec3 = vec1 + vec2;
		std::cout << "vec1 + vec2 = " << vec3.x() << " " << vec3.y() << " " << vec3.z() << " " << vec3.w() << std::endl;

		vec3 = vec2 - vec1;
		std::cout << "vec2 - vec1 = " << vec3.x() << " " << vec3.y() << " " << vec3.z() << " " << vec3.w() << std::endl;

		float dotProduct = Engine::Math::dot(vec1 , vec2);
		std::cout << "dot product of vec1 and vec2 = " << dotProduct << std::endl;

		vec3 = vec2 / 3;
		std::cout << "vec2 / 3 = " << vec3.x() << " " << vec3.y() << " " << vec3.z() << " " << vec3.w() << std::endl;

		vec3 = vec1 * 2;
		std::cout << "vec1 * 2 = " << vec3.x() << " " << vec3.y() << " " << vec3.z() << " " << vec3.w() << std::endl;

		std::cout << "Length of vec 1 = " << vec1.Length() << std::endl;
		std::cout << "Squared length of vec 2 = " << vec2.LengthSq() << std::endl;

		vec3.Normalize();
		std::cout << "Normalized vec1 = " << vec3.x() << " " << vec3.y() << " " << vec3.z() << " " << vec3.w() << std::endl;
	}
}