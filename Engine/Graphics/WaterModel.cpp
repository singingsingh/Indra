#include <Engine\Graphics\WaterModel.h>

#include <External\Assimp\include\Importer.hpp>
#include <External\Assimp\include\postprocess.h>
#include <External\Assimp\include\scene.h>

#include <Engine\Util\ConsolePrint.h>
#include <Engine\Graphics\GraphicsDX.h>
#include <Engine\System\Timer.h>
#include <Engine\Util\Assert.h>
#include <Engine\Util\MathUtils.h>
#include <Engine\Core\MemoryMgr.h>

#include <math.h>

#include <fstream>

namespace
{
	uint64_t currentTick;
}

namespace Engine
{
	namespace Graphics
	{
		WaterModel::WaterModel()
			:_numWaveParticles(400),
			_waveParticleMemPool(static_cast<WaveParticle*>(MemoryMgr::getInstance()->allocMemory(_numWaveParticles * sizeof(WaveParticle))))
		{
			_vertexBuffer = nullptr;
			_indexBuffer = nullptr;
			_vertices = nullptr;
			_indices = nullptr;
			_gridWidth = 0.1f;
			_gridHeight = 0.1f;
			_gridRows = 64;
			_gridCols = 64;
			_activeParticles = 0;

			_freeList = nullptr;
			_activeList = nullptr;

			_corner = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			float halfWidth = _gridCols*_gridWidth * 0.5f;
			float halfHeight = _gridRows*_gridHeight * 0.5f;
			_corner.x = -halfWidth;
			_corner.z = halfHeight;

			initializeWaveParticles();
		}

		WaterModel::~WaterModel()
		{
		}

		bool WaterModel::initialize()
		{
			bool result;

			// Initialize the vertex and index buffer that hold the geometry for the triangle.
			result = initializeBuffers();
			if (!result)
			{
				return false;
			}

			return true;
		}

		void WaterModel::shutdown()
		{
			shutdownBuffers();
			releaseModel();
			MemoryMgr::getInstance()->dellocMemory(static_cast<void*>(const_cast<WaveParticle*>(_waveParticleMemPool)));
		}

		void WaterModel::render()
		{
			currentTick = System::Timer::GetCurrentTick();
			subDivideParticles();
			updateBuffers();
			renderBuffers();
		}

		int WaterModel::getIndexCount()
		{
			return _indexCount;
		}

		void WaterModel::spawnParticles()
		{
			WaveParticle* newParticles = getFreePartices(3);
			WaveParticle* first = newParticles;

			newParticles->origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			newParticles->velocity = 0.002f;
			newParticles->amplitude = 0.2f;
			newParticles->radius = 0.5f;
			newParticles->angle = 30.0f;
			newParticles->spawnTick = System::Timer::GetCurrentTick();
			newParticles->actionTick = newParticles->spawnTick + System::Timer::ConvertMilliSecToTick( newParticles->radius / (4.0 * sin ((newParticles->angle / 2.0)*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;
			newParticles->origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR3(sin(30.0f*MathUtils::DegToRad), 0.0f, cos(30.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.002f;
			newParticles->amplitude = 0.2f;
			newParticles->radius = 0.5f;
			newParticles->angle = 30.0f;
			newParticles->spawnTick = System::Timer::GetCurrentTick();
			newParticles->actionTick = newParticles->spawnTick + System::Timer::ConvertMilliSecToTick(newParticles->radius / (4.0 * sin((newParticles->angle / 2.0)*MathUtils::DegToRad) * newParticles->velocity));

			newParticles = newParticles->next;
			newParticles->origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			newParticles->direction = D3DXVECTOR3(sin(-30.0f*MathUtils::DegToRad), 0.0f, cos(-30.0f*MathUtils::DegToRad));
			newParticles->velocity = 0.002f;
			newParticles->amplitude = 0.2f;
			newParticles->radius = 0.5f;
			newParticles->angle = 30.0f;
			newParticles->spawnTick = System::Timer::GetCurrentTick();
			newParticles->actionTick = newParticles->spawnTick + System::Timer::ConvertMilliSecToTick(newParticles->radius / (4.0 * sin((newParticles->angle / 2.0)*MathUtils::DegToRad) * newParticles->velocity));
			newParticles->next = nullptr;

			pushToActiveList(first);
		}

		bool WaterModel::initializeBuffers()
		{
			D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData, indexData;
			HRESULT result;

			_vertexCount = (_gridCols + 1)*(_gridRows + 1);
			_indexCount = (_gridCols*_gridRows) * 2 * 3;

			_vertices = new VertexType[_vertexCount];
			if (!_vertices)
			{
				return false;
			}

			_indices = new unsigned long[_indexCount];
			if (!_indices)
			{
				return false;
			}

			int vertexCount = 0;
			for (uint8_t row = 0; row <= _gridRows; row++)
			{
				for (uint8_t col = 0; col <= _gridCols; col++)
				{
					_vertices[vertexCount].position = D3DXVECTOR3(col*_gridWidth, 0.0f, -row*_gridHeight) + _corner;
					_vertices[vertexCount].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					vertexCount++;
				}
			}

			int indexCount = 0;
			for (uint8_t row = 0; row < _gridRows; row++)
			{
				for (uint8_t col = 0; col < _gridCols; col++)
				{
					_indices[indexCount] = row * (_gridCols + 1) + col;
					indexCount++;

					_indices[indexCount] = (row + 1) * (_gridCols + 1) + col + 1;
					indexCount++;

					_indices[indexCount] = (row + 1) * (_gridCols + 1) + col;
					indexCount++;

					_indices[indexCount] = row * (_gridCols + 1) + col;
					indexCount++;

					_indices[indexCount] = row * (_gridCols + 1) + col + 1;
					indexCount++;

					_indices[indexCount] = (row + 1) * (_gridCols + 1) + col + 1;
					indexCount++;
				}
			}

			// Set up the description of the static vertex buffer.
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the vertex data.
			vertexData.pSysMem = _vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			ID3D11Device* device = GraphicsDX::GetDevice();
			// Now create the vertex buffer.
			result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			// Set up the description of the static index buffer.
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			// Give the subresource structure a pointer to the index data.
			indexData.pSysMem = _indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer.
			result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
			if (FAILED(result))
			{
				return false;
			}

			return true;
		}

		void WaterModel::shutdownBuffers()
		{
			// Release the index buffer.
			if (_indexBuffer)
			{
				_indexBuffer->Release();
				_indexBuffer = 0;
			}

			// Release the vertex buffer.
			if (_vertexBuffer)
			{
				_vertexBuffer->Release();
				_vertexBuffer = 0;
			}

			// Release the arrays now that the vertex and index buffers have been created and loaded.
			delete[] _vertices;
			_vertices = 0;

			delete[] _indices;
			_indices = 0;

			return;
		}

		void WaterModel::renderBuffers()
		{
			unsigned int stride;
			unsigned int offset;

			stride = sizeof(VertexType);
			offset = 0;

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void WaterModel::releaseModel()
		{
		}

		void WaterModel::subDivideParticles()
		{
			WaveParticle* currentParticle = _activeList;

			while (currentParticle && currentParticle->actionTick < currentTick)
			{
				_activeList = currentParticle->next;

				WaveParticle* newParticles = getFreePartices(2);
				currentParticle->next = newParticles;

				currentParticle->amplitude = currentParticle->amplitude * 0.33333333f;
				currentParticle->angle = currentParticle->angle * 0.33333333f;
				currentParticle->actionTick = currentParticle->actionTick +
					System::Timer::ConvertMilliSecToTick(currentParticle->radius / (4.0 * sin((currentParticle->angle * 0.5)*MathUtils::DegToRad) * currentParticle->velocity));

				WaveParticle* temp = newParticles;
				while (temp)
				{
					temp->origin = currentParticle->origin;
					temp->spawnTick = currentParticle->spawnTick;
					temp->actionTick = currentParticle->actionTick;
					temp->angle = currentParticle->angle;
					temp->amplitude = currentParticle->amplitude;
					temp->radius = currentParticle->radius;
					temp->velocity = currentParticle->velocity;

					temp = temp->next;
				}

				D3DXVECTOR3 direction = currentParticle->direction;
				D3DXVECTOR3 newDirection = direction;
				float angle = currentParticle->angle*MathUtils::DegToRad;
				newDirection.x = direction.x * cos(angle) - direction.z * sin(angle);
				newDirection.z = direction.x * sin(angle) + direction.z * cos(angle);
				temp = newParticles;
				temp->direction = newDirection;

				temp = temp->next;

				direction = currentParticle->direction;
				newDirection = currentParticle->direction;
				angle = -currentParticle->angle*MathUtils::DegToRad;
				newDirection.x = direction.x * cos(angle) - direction.z * sin(angle);
				newDirection.z = direction.x * sin(angle) + direction.z * cos(angle);
				temp->direction = newDirection;

				pushToActiveList(currentParticle);

				currentParticle = _activeList;
			}
		}

		void WaterModel::updateBuffers()
		{
			WaveParticle* currentParticle = _activeList;
			bool first = true;

			while (currentParticle)
			{
				double elapsedTime = System::Timer::GetElapsedTimeMilliSec(currentParticle->spawnTick, currentTick, false);
				D3DXVECTOR3 wavePos = currentParticle->origin + (currentParticle->direction * currentParticle->velocity * (float)elapsedTime);
				int vertexCount = 0;
				float invRadius = 1.0f / currentParticle->radius;

				for (uint8_t row = 0; row <= _gridRows; row++)
				{
					for (uint8_t col = 0; col <= _gridCols; col++)
					{
						if (first)
						{
							_vertices[vertexCount].position = D3DXVECTOR3(col*_gridWidth, 0.0f, -row*_gridHeight) + _corner;
						}

						float dist = sqrt(((_vertices[vertexCount].position.x - wavePos.x) * (_vertices[vertexCount].position.x - wavePos.x))
							+ ((_vertices[vertexCount].position.z - wavePos.z) * (_vertices[vertexCount].position.z - wavePos.z)));

						float rectFunc = dist * invRadius;
						if (rectFunc <= 1.0f)
						{
							_vertices[vertexCount].position.y += currentParticle->amplitude * 0.5f * (cos(MathUtils::PI * invRadius * dist) + 1.0f);
						}

						vertexCount++;
					}
				}

				first = false;
				currentParticle = currentParticle->next;
			}

			// update the buffers

			ID3D11DeviceContext* deviceContext = GraphicsDX::GetDeviceContext();
			HRESULT result;
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				MessagedAssert(false, "Not able to map vertex buffer in water model.");
			}

			memcpy(mappedResource.pData, _vertices, sizeof(VertexType)*_vertexCount);
			deviceContext->Unmap(_vertexBuffer, 0);
		}

		void WaterModel::initializeWaveParticles()
		{
			_freeList = const_cast<WaveParticle*>(_waveParticleMemPool);
			WaveParticle* currentPtr = _freeList;
			WaveParticle* nextPtr = currentPtr + 1;

			for (uint32_t i = 0; i < _numWaveParticles - 1; i++)
			{
				currentPtr->next = nextPtr;
				currentPtr = nextPtr;
				nextPtr += 1;
			}

			currentPtr->next = nullptr;
		}

		WaterModel::WaveParticle * WaterModel::getFreePartices(uint32_t i_numParticles)
		{
			if (_freeList == nullptr)
			{
				MessagedAssert(false, "Free particles exhausted.");
				return nullptr;
			}

			WaveParticle* first = _freeList;
			WaveParticle* current = first;
			WaveParticle* prev = nullptr;

			uint32_t i = 0;
			for (; i < i_numParticles && current != nullptr; i++)
			{
				prev = current;
				current = current->next;
			}

			if (i == i_numParticles)
			{
				prev->next = nullptr;
				_freeList = current;
				return first;
			}
			else
			{
				MessagedAssert(false, "Free particles exhausted.");
				return nullptr;
			}
		}

		void WaterModel::pushToActiveList(WaveParticle * i_waveParticle)
		{
			WaveParticle* currentParticle = i_waveParticle;
			WaveParticle* nextParticle = nullptr;

			while (currentParticle)
			{
				nextParticle = currentParticle->next;
				WaveParticle* currentPtr = _activeList;
				WaveParticle* prevPtr = nullptr;

				while (currentPtr && currentParticle->actionTick > currentPtr->actionTick)
				{
					prevPtr = currentPtr;
					currentPtr = currentPtr->next;
				}

				if (prevPtr)
				{
					prevPtr->next = currentParticle;
					currentParticle->next = currentPtr;
				}
				else
				{
					_activeList = currentParticle;
					currentParticle->next = nullptr;
				}

				currentParticle = nextParticle;
			}
		}

		void WaterModel::recycleParticles(WaveParticle* i_waveParticle)
		{
			if (i_waveParticle == nullptr)
			{
				return;
			}

			WaveParticle* last = i_waveParticle;

			while (last->next != nullptr)
			{
				last = last->next;
			}

			last->next = _freeList;
			_freeList = i_waveParticle;
		}
	}
}