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

			void setDiffuseColor(float r, float g, float b, float a);
			void setDirection(float x, float y, float z);

			D3DXVECTOR4 getDiffuseColor();
			D3DXVECTOR3 getDirection();

		private:
			D3DXVECTOR4 _diffuseColor;
			D3DXVECTOR3 _direction;
		};
	}
}

#endif	//__DIFFUSE_LIGHT_H__
