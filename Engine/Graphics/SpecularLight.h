#ifndef __SPECULAR_LIGHT_H__
#define __SPECULAR_LIGHT_H__

#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class SpecularLight
		{
			public:
				SpecularLight();
				SpecularLight(const SpecularLight&);
				~SpecularLight();

				void setAmbientColor(float, float, float, float);
				void setDiffuseColor(float, float, float, float);
				void setDirection(float, float, float);
				void setSpecularColor(float, float, float, float);
				void setSpecularPower(float);

				D3DXVECTOR4 getAmbientColor();
				D3DXVECTOR4 getDiffuseColor();
				D3DXVECTOR3 getDirection();
				D3DXVECTOR4 getSpecularColor();
				float getSpecularPower();

			private:
				D3DXVECTOR4 _ambientColor;
				D3DXVECTOR4 _diffuseColor;
				D3DXVECTOR3 _direction;
				D3DXVECTOR4 _specularColor;
				float _specularPower;
		};
	}
}

#endif	//__SPECULAR_LIGHT_H__