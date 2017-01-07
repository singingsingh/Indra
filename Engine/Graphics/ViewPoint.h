#ifndef __VIEW_POINT_H__
#define __VIEW_POINT_H__


#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class ViewPoint
		{
		public:
			ViewPoint();
			~ViewPoint();

			void SetPosition(float, float, float);
			void SetLookAt(float, float, float);
			void SetProjectionParameters(float, float, float, float);

			void GenerateViewMatrix();
			void GenerateProjectionMatrix();

			void GetViewMatrix(D3DXMATRIX&);
			void GetProjectionMatrix(D3DXMATRIX&);

		private:
			D3DXVECTOR3 m_position, m_lookAt;
			D3DXMATRIX m_viewMatrix, m_projectionMatrix;
			float m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane;
		};
	}	// 
}

#endif	// __VIEW_POINT_H__