#include <Engine\Util\Assert.h>
#include <Engine\Util\Assert.h>

namespace Engine
{
	namespace Math
	{
		// Allows us to use V = V * M; (i.e. row vector)
		inline Vector3 operator*( const Matrix3 & i_mtx, const Vector3 & i_vec )
		{
			return i_mtx.MultiplyRight( i_vec );
		}

		// Allows us to use V = M * V; (i.e. column vector)
		inline Vector3 operator*( const Vector3 & i_vec, const Matrix3 & i_mtx )
		{
			return i_mtx.MultiplyLeft( i_vec );
		}

		inline Matrix3 Matrix3::operator*( const Matrix3 & i_other ) const
		{
			Assert( !MathUtils::IsNAN( m_11 ) );
	
			Matrix3 tResult;
			Multiply( i_other, tResult );

			return Matrix3( tResult );
		}
	} // namespace Math
} // namespace Engine