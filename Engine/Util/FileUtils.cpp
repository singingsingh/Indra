#include <Engine\Util\FileUtils.h>
#include <Engine\Util\Assert.h>
#include <stdio.h>

namespace Engine
{
	void* FileUtils::LoadFile(const char * i_pFilename, size_t & o_sizeFile)
	{
		Assert(i_pFilename != NULL);

		FILE * pFile = NULL;

		errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
		if (fopenError != 0)
		{
			MessagedAssert(false, "File not found.\n");
			return NULL;
		}

		Assert(pFile != NULL);

		int FileIOError = fseek(pFile, 0, SEEK_END);
		Assert(FileIOError == 0);

		long FileSize = ftell(pFile);
		Assert(FileSize >= 0);

		FileIOError = fseek(pFile, 0, SEEK_SET);
		Assert(FileIOError == 0);

		uint8_t * pBuffer = new uint8_t[FileSize];
		Assert(pBuffer);

		size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
		Assert(FileRead == FileSize);

		fclose(pFile);

		o_sizeFile = FileSize;

		return pBuffer;
	}

	bool FileUtils::FileExits( const char* fileName )
	{
		Assert(fileName != NULL);

		FILE * pFile = NULL;

		errno_t fopenError = fopen_s(&pFile, fileName, "rb");
		if (fopenError != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}	// namespace Engine