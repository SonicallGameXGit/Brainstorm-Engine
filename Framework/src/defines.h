#pragma once

#ifdef BS_BUILD_DLL
	#define BS_API _declspec(dllexport)
#else
	#define BS_API _declspec(dllimport)
#endif