#pragma once

#include	"Python.h"
#include	"pybind11/pybind11.h"
#include	"Location.hpp"

typedef struct {
	void(*WriteReport_Alert)(const char* format, ...);






	//-------- IO --------

	/*this function only used in class constructor functions of pybind export module.
	  please delete outData on class disposed  */
	IO::Name (*ReadFile)(GS::UniString &relativePath, char **outData, UINT32 *outSize);

}ExportFuns;


#define APP_INFO_FLAGS_IS_INIT		0x01
#define APP_INFO_FLAGS_IS_REBOOT	0x02
#define APP_INFO_NAME_MAX			64
#define APP_INFO_VERSION			0x00000001

typedef struct {
	UINT32			iVersion;
	int				ID;
	char			Name[APP_INFO_NAME_MAX];

	UINT8			iFlags;
	UINT32			iFunsSize;
	PyThreadState	*iState;
	PyObject		*iMain;
	ExportFuns		*iFuns;
}APP_Info;

