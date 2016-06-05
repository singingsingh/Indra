#ifndef __FILE_UTILS_H__

#define __FILE_UTILS_H__

namespace Engine
{
	class FileUtils
	{
		public :
			static void* LoadFile(const char * i_pFilename, size_t & o_sizeFile);
			static bool FileExits( const char* fileName );
	};
}

#endif	//__FILE_UTILS_H__
