#include <Engine\Core\Math\Matrix4.h>
#include <Engine\Util\MathUtils.h>

#include <iostream>

namespace UnitTest
{
	void printMatrix( Engine::Math::Matrix4& i_matrix4 )
	{
		Engine::Math::Vector4 temp;
		for (int i = 0; i < 4; i++)
		{
			temp = i_matrix4.Row(i);
			std::cout << temp.x() << " " << temp.y() << " " << temp.z() << " " << temp.w() << std::endl;
		}
		std::cout << std::endl;
	}

	void Matrix4UT()
	{
		std::cout << "Matrix 4 unit test" << std::endl;

		Engine::Math::Matrix4 mat = Engine::Math::Matrix4::CreateIdentity();
		std::cout << "Identity Matrix:\n";
		printMatrix(mat);

		Engine::Math::Matrix4 scaleMat = Engine::Math::Matrix4::CreateScale( 1.0f, 2.0f, 3.0f );
		std::cout << "Scalar Matrix:\n";
		printMatrix( scaleMat );

		Engine::Math::Matrix4 rotMat = Engine::Math::Matrix4::CreateXRotationCV(Engine::MathUtils::DegToRad * 65.0f);
		std::cout << "Rotation Matrix for 65 degrees:\n";
		printMatrix( rotMat );

		Engine::Math::Matrix4 transposeMat = rotMat.GetTranspose();
		std::cout << "Transpose of the above rotational matrix :\n";
		printMatrix( transposeMat );

		Engine::Math::Matrix4 transMat = Engine::Math::Matrix4::CreateTranslationCV(1.0f, 2.0f, 3.0f);
		std::cout << "Translation Matrix to the Point 1.0f, 2.0f, 3.0f :\n";
		printMatrix( transMat );

		Engine::Math::Matrix4 invMat = transMat.GetInverse();
		std::cout << "Inverse of the Translation Matrix 1.0f, 2.0f, 3.0f :\n";
		printMatrix( invMat );

		Engine::Math::Matrix4 mulMat = transMat * rotMat * scaleMat;
		std::cout << "Multiplication of the above translation, rotation, scale matrices :\n";
		printMatrix( mulMat );

		Engine::Math::Vector4 vec( 2.0f, 3.0f, 5.0f, 1.0f );
		Engine::Math::Vector4 newVec = mulMat.MultiplyRight( vec );
		std::cout << "Vector 2.0f, 3.0f, 5.0f after Tranformed by mulMat :\n";
		std::cout << newVec.x() << " " << newVec.y() << " " << newVec.z() << " " << newVec.w() << std::endl;
	}
}