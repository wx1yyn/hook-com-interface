#include "stdafx.h"
#include <WinError.h>

#include "ComSample_i.h"
#include "ComSample_i.c"

void CallSampleObject()
{
    ATL::CComPtr<ISampleObject> sampleObject;
    HRESULT hr = sampleObject.CoCreateInstance(CLSID_SampleObject);
    if (FAILED(hr))
        return;

    ATL::CComBSTR name;
    sampleObject->get_ObjectName(&name);
    std::wcout << "Object name: " << (wchar_t*)name << std::endl;

    LONG result;
    sampleObject->DoWork(10, 20, &result);
    std::wcout << "Work result: " << result;
}

int main(int argc, char* argv[])
{
	//loadlib
	//DWORD dwError = NO_ERROR;
	//HINSTANCE hInst;
	//hInst=LoadLibrary(TEXT("./ComInterceptVtablePatch.dll"));
	//dwError = GetLastError();
	printf("xxxxx\n");
    CoInitialize(NULL);
    CallSampleObject();
    CoUninitialize();
	getchar();

    return 0;
}

