#ifndef _Camera_H_
#define _Camera_H_

#include <d3dx10math.h>

namespace Engine
{
	namespace Graphics
	{
		class Camera
		{
		public:
			Camera(float nearPlane, float farPlane, float fov, float aspectRatio);
			Camera(const Camera&);
			~Camera();

			void setPosition(float, float, float);
			void setRotation(float, float, float);

			D3DXVECTOR3 getPosition();
			D3DXVECTOR3 getRotation();

			void update();
			D3DXMATRIX getViewMatrix();
			D3DXMATRIX getProjectionMatrix();
			D3DXMATRIX getOrthogonalMatrix();

		private:

			float _nearPlane;
			float _farPlane;
			float _fov;
			float _aspectRatio;

			D3DXVECTOR3 _position, _rotation;
			D3DXMATRIX _viewMatrix;
			D3DXMATRIX _orthoMatrix;
			D3DXMATRIX _projectionMatrix;
		};
	}
}

#endif	//_Camera_H_