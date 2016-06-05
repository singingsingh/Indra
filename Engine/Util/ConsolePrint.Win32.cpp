#include <stdarg.h>		// for va_<xxx>
#include <stdio.h>		// for vsprintf_s()
#include <Windows.h>	// for OutputDebugStringA(). Uggh.. this pulls in a lot of Windows specific stuff

namespace Engine
{
	void info(const char * file, unsigned int lineNo)
	{
		#ifdef VERBOSE_DEBUG_PRINT

		const size_t lenTemp = 512;
		char strTemp[lenTemp];

			#ifdef VERBOSE_EXTREME_DEBUG_PRINT
				#ifdef WIN32
						sprintf_s(strTemp, "DEBUG: File : %s, line : %d, System : Windows\n", file, lineNo);
				#else
				#error "Unknown platform."
				#endif
			#else
					sprintf_s(strTemp, "DEBUG: File : %s, line : %d\n", file, lineNo);
			#endif
		OutputDebugStringA(strTemp);
		#endif
	}

	void ConsolePrint(const char * i_fmt, ...)
	{
		const size_t lenTemp = 512;
		char strTemp[lenTemp] = "DEBUG: ";

		strcat_s(strTemp, i_fmt);

		const size_t lenOutput = lenTemp + 32;

		char strOutput[lenOutput];

		// define a variable argument list variable 
		va_list args;
	
		// initialize it. second parameter is variable immediately
		// preceeding variable arguments
		va_start(args, i_fmt);

		// (safely) print our formatted string to a char buffer
		vsprintf_s(strOutput, lenOutput, strTemp, args);

		va_end(args);

		OutputDebugStringA(strOutput);
		//OutputDebugStringA("\n");
	}

} // namespace Engine