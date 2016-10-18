#include <Engine\Graphics\Camera.h>

#include <Engine\System\Window.h>

namespace Engine
{
	namespace Graphics
	{
		Camera::Camera(float i_nearPlane, float i_farPlane, float i_fov, float i_aspectRatio)
		{
			_nearPlane = i_nearPlane;
			_farPlane = i_farPlane;
			_fov = i_fov;
			_aspectRatio = i_aspectRatio;

			D3DXMatrixPerspectiveFovLH(&_projectionMatrix, _fov, _aspectRatio, i_nearPlane, i_farPlane);
			D3DXMatrixOrthoLH(&_orthoMatrix, float(System::Window::GetWidth()), float(System::Window::GetHeight()), i_nearPlane, i_farPlane);

			_position.x = 0;
			_position.y = 0;
			_position.z = 0;

			_rotation.x = 0;
			_rotation.y = 0;
			_rotation.z = 0;
		}

		Camera::Camera(const Camera &)
		{
		}

		Camera::~Camera()
		{
		}

		void Camera::setPosition(float i_x, float i_y, float i_z)
		{
			_position.x = i_x;
			_position.y = i_y;
			_position.z = i_z;
		}

		void Camera::setRotation(float i_x, float i_y, float i_z)
		{
			_rotation.x = i_x;
			_rotation.y = i_y;
			_rotation.z = i_z;
		}

		D3DXVECTOR3 Camera::getPosition()
		{
			return _position;
		}

		D3DXVECTOR3 Camera::getRotation()
		{
			return _rotation;
		}

		void Camera::update()
		{
			D3DXVECTOR3 up, lookAt;
			float yaw, pitch, roll;
			D3DXMATRIX rotationMatrix;


			// Setup the vector that points upwards.
			up.x = 0.0f;
			up.y = 1.0f;
			up.z = 0.0f;


			// Setup where the camera is looking by default.
			lookAt.x = 0.0f;
			lookAt.y = 0.0f;
			lookAt.z = 1.0f;

			// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
			pitch = _rotation.x * 0.0174532925f;
			yaw = _rotation.y * 0.0174532925f;
			roll = _rotation.z * 0.0174532925f;

			// Create the rotation matrix from the yaw, pitch, and roll values.
			D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

			// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
			D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
			D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

			// Translate the rotated camera position to the location of the viewer.
			lookAt = _position + lookAt;

			// Finally create the view matrix from the three updated vectors.
			D3DXMatrixLookAtLH(&_viewMatrix, &_position, &lookAt, &up);
		}

		D3DXMATRIX Camera::getViewMatrix()
		{
			return _viewMatrix;
		}

		D3DXMATRIX Camera::getProjectionMatrix()
		{
			return _projectionMatrix;
		}

		D3DXMATRIX Camera::getOrthogonalMatrix()
		{
			return _orthoMatrix;
		}
	}
}	// namespace Engine
