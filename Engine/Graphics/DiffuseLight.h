#ifndef __DIFFUSE_LIGHT_H__
#define __DIFFUSE_LIGHT_H__

#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class DiffuseLight
		{
		public:
			DiffuseLight();
			~DiffuseLight();
			
			void setAmbientColor(float r, float g, float b, float a);
			void setDiffuseColor(float r, float g, float b, float a);
			void setDirection(float x, float y, float z);
			void SetPosition(float, float, float);
			void SetLookAt(float, float, float);

			D3DXVECTOR4 getDiffuseColor();
			D3DXVECTOR4 getAmbientColor();
			D3DXVECTOR3 getDirection();
			D3DXVECTOR3 getPosition();

			void GenerateViewMatrix();
			void GenerateProjectionMatrix(float, float);

			void generateDirection();

			void GetViewMatrix(D3DXMATRIX&);
			void GetProjectionMatrix(D3DXMATRIX&);

		private:
			D3DXVECTOR4 _diffuseColor;
			D3DXVECTOR4 _ambientColor;
			D3DXVECTOR3 _direction;
			D3DXVECTOR3 m_position;
			D3DXVECTOR3 m_lookAt;
			D3DXMATRIX m_viewMatrix;
			D3DXMATRIX m_projectionMatrix;
		};
	}
}

#endif	//__DIFFUSE_LIGHT_H__
