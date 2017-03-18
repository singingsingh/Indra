#include <Engine\Graphics\Texture3D.h>

#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\Util\Assert.h>

#include <IL\il.h>

namespace Engine
{
	namespace Graphics
	{
		Texture3D::Texture3D(const char * i_fileName, unsigned short i_width, unsigned short i_height, unsigned short i_depth)
		{
			D3D11_TEXTURE3D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));

			textureDesc.Width = i_width;
			textureDesc.Height = i_height;
			textureDesc.Depth = i_depth;
			textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			unsigned char* tex3dData = (unsigned char*)malloc(i_width*i_height*i_depth*4);
			unsigned char* currPos = tex3dData;
			// load the texture using devil
			{
				ILboolean success;
				ilInit();
				ILuint imageIds;
				ilGenImages(1, &imageIds);

				ilBindImage(imageIds); /* Binding of DevIL image name */
				ilEnable(IL_ORIGIN_SET);
				ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
				success = ilLoadImage((ILstring)i_fileName);
				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				unsigned char* slicePtr = (unsigned char*)malloc(i_width*i_height * 4);

				for (unsigned int row = 0; row < 8; row++)
				{
					for (unsigned int col = 1; col < 8; col++)
					{
						ilCopyPixels(col*64, row*64, 0, 64, 64, 1, IL_RGBA, IL_UNSIGNED_BYTE, slicePtr);
						memcpy(currPos, slicePtr, 64*64*4);
						currPos += 64 * 64 * 4;
						break;
					}
					break;
				}
			}

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = (void*)tex3dData;
			initData.SysMemPitch = 64 * 4;
			initData.SysMemSlicePitch = 64 * 64 * 4;

			HRESULT result = GraphicsDX::GetDevice()->CreateTexture3D(&textureDesc, &initData, &_3DTextureBuffer);
			MessagedAssert(SUCCEEDED(result), "Failed to create 3D texture.");

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			result = GraphicsDX::GetDevice()->CreateShaderResourceView(_3DTextureBuffer, &shaderResourceViewDesc, &_3DTextureSRV);
			MessagedAssert(SUCCEEDED(result), "Failed to create 3D texture Shader resource view");


			{
				//D3DX11_IMAGE_LOAD_INFO imageLoadInfo;
				//ZeroMemory(&imageLoadInfo, sizeof(imageLoadInfo));

				//imageLoadInfo.Width = i_width;
				//imageLoadInfo.Height = i_height;
				//imageLoadInfo.Depth = i_depth;
				//imageLoadInfo.FirstMipLevel = 0;
				//imageLoadInfo.MipLevels = 1;
				//imageLoadInfo.Usage = D3D11_USAGE_DEFAULT;
				//imageLoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				//imageLoadInfo.CpuAccessFlags = 0;
				//imageLoadInfo.MiscFlags = 0;
				//imageLoadInfo.Format = DXGI_FORMAT_FROM_FILE;
				//imageLoadInfo.Filter = D3DX11_FILTER_LINEAR;
				//imageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;

				//D3DX11_IMAGE_INFO imageInfo;

				//D3DX11GetImageInfoFromFile(i_fileName, NULL, &imageInfo, NULL);

				//imageLoadInfo.pSrcInfo = &imageInfo;

				//HRESULT result = D3DX11CreateShaderResourceViewFromFile(GraphicsDX::GetDevice(), i_fileName, &imageLoadInfo, NULL, &_3DTextureSRV, NULL);
				//MessagedAssert(SUCCEEDED(result), "Failed to create 3D texture Shader resource view");
			}

		}

		Texture3D::~Texture3D()
		{
			_3DTextureSRV->Release();
			_3DTextureSRV = nullptr;
		}

		ID3D11ShaderResourceView * Texture3D::getTexture() const
		{
			return _3DTextureSRV;
		}
	}
}