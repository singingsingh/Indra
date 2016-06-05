#ifndef __SPRITE_DATA_TYPES_H__
#define __SPRITE_DATA_TYPES_H__

#include <stdint.h>
#include <d3dtypes.h>

namespace GraphicsDX
{
	namespace SpriteDataTypes
	{
		struct UV
		{
			float			u, v;
		};

		struct RGBA
		{
			uint8_t			r, g, b, a;
		};

		typedef struct _SpriteEdges
		{
			float			Left;
			float			Top;
			float			Right;
			float			Bottom;
		} SpriteEdges;

		typedef struct _SpriteUVs
		{
			UV				TopLeft;
			UV				TopRight;
			UV				BottomLeft;
			UV				BottomRight;
		} SpriteUVs;

		typedef struct _SpriteVertex
		{
			float			Position[3];
			D3DCOLOR		Color;
			float			UV[2];
		}SpriteVertex;
	} // namespace Sprites
} // namespace GraphicsDX

#endif //__SPRITE_DATA_TYPES_H__