#ifndef __REAL_3D_TEX_SHADER_H__
#define __REAL_3D_TEX_SHADER_H__

#include <Engine\Graphics\SpecularModel.h>
#include <Engine\Engine\KeyboardNotifier.h>

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace Engine
{
	namespace Graphics
	{
		class Real3DTexShader : public Engine::IKeyboardListener
		{
			public:
				static Real3DTexShader* createReal3DTexShader();
				~Real3DTexShader();
				void render(SpecularModel* i_meshModel);

				virtual void keyboardUpdate(uint8_t key, bool down, uint16_t x, uint16_t y);
				virtual void mouseClickUpdate(uint8_t button, bool down, uint16_t x, uint16_t y);
				virtual void mouseMoveUpdate(bool leftBt, bool rightBt, bool middleBt, uint16_t x, uint16_t y);
				virtual void mousePassiveMoveUpdate(uint16_t x, uint16_t y);
				virtual void mouseWheelUpdate(bool direction, uint16_t x, uint16_t y);

			private:

				struct MatrixBufferType
				{
					D3DXMATRIX world;
					D3DXMATRIX view;
					D3DXMATRIX projection;
				};

				struct TextureBufferType
				{
					float layer;
					D3DXVECTOR3 padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
				};

				Real3DTexShader();
				bool initialize();

				bool initializeShader(const char * vsFilename, const char *psFilename);
				void shutdownShader();
				void outputShaderErrorMessage(ID3D10Blob* errorMessage, const char * shaderFilename);

				void setShaderParameters(SpecularModel* i_specularModel);
				void renderShader(int indexCount);

				ID3D11VertexShader* _vertexShader;
				ID3D11PixelShader* _pixelShader;
				ID3D11InputLayout* _layout;
				ID3D11SamplerState* _sampleState;
				ID3D11Buffer* _matrixBuffer;
				ID3D11Buffer* _lightBuffer;

				float _zValue;
				float _zValueStep;
		};
	}	// namespace Graphics
}	// namespace Engine

#endif	//__REAL_3D_TEX_SHADER_H__