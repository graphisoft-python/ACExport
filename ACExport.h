#pragma once

#include	"Python.h"
#include	"pybind11/pybind11.h"
#include	"Location.hpp"
#include	"Definitions.hpp"
#include	"APIdefs_Environment.h"

typedef struct {
	void(*WriteReport_Alert)(const char* format, ...);

	GSErrCode (*ACAPI_Environment)(API_EnvironmentID code, void* par1, void* par2, void* par3);




	//-------- Application --------
	IO::Location(*ApplicationPath)();

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

