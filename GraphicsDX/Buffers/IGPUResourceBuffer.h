#pragma once

#include <d3d11.h>

namespace GraphicsDX
{
	class IGPUResourceBuffer
	{
	public:
		enum eUsage
		{
			USAGE_VERTICES = 0,
			USAGE_INDICES
		};

		virtual ID3D11Buffer * GetDeviceBuffer() = 0;
	};

} // namespace GraphicsDX
