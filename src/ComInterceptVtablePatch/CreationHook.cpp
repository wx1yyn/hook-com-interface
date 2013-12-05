#include "StdAfx.h"
#include "CreationHook.h"
//#include "Factory.h"
#include "VtableHooks.h"
#include <WinError.h>
//////////////////////////////////////////////////////////////////////////

const CLSID CLSID_AudioClient = {0x41FCCC3A,0x1FA1,0x4949,{0x95,0x3A,0x6E,0xE6,0x1C,0x46,0xA4,0xD1}};

//typedef HRESULT (WINAPI *CoCreateInstance_T)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*);
//typedef HRESULT (WINAPI *CoGetClassObject_T)(REFCLSID rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo, REFIID riid, LPVOID *ppv);
typedef HRESULT (WINAPI *DllGetClassObject_T)(REFCLSID rclsid, REFIID riid, LPVOID** ppv);

namespace Original
{
    //CoCreateInstance_T  CoCreateInstance    = NULL;
    //CoGetClassObject_T  CoGetClassObject    = NULL;
	DllGetClassObject_T	 DllGetClassObject = NULL;
}

namespace Hook
{
    //HRESULT WINAPI CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv);
    //HRESULT WINAPI CoGetClassObject(REFCLSID rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo, REFIID riid, LPVOID *ppv);
	HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID** ppv);
};

//////////////////////////////////////////////////////////////////////////

struct FunctionInfo
{
    char*  FunctionModule;
    char*  FunctionName;
    void** OriginalFunction;
    void*  HookFunction;
};

FunctionInfo g_Functions[] = 
{
    //{"ole32.dll", "CoCreateInstance", (void**)&Original::CoCreateInstance, (void*)Hook::CoCreateInstance}
    //{"ole32.dll", "CoGetClassObject", (void**)&Original::CoGetClassObject, (void*)Hook::CoGetClassObject}
	{"Audioses.dll", "DllGetClassObject", (void**)&Original::DllGetClassObject, (void*)Hook::DllGetClassObject}
};

const size_t g_FunctionsCount = sizeof(g_Functions)/sizeof(FunctionInfo);

//////////////////////////////////////////////////////////////////////////

//HRESULT WINAPI Hook::CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv)
//{
//    if (rclsid == CLSID_xxxxxx)
//    {        
//        if (pUnkOuter)
//            return CLASS_E_NOAGGREGATION;
//
//        HRESULT hr = Original::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
//        if (FAILED(hr))
//            return hr;
//
//        return InstallComInterfaceHooks((IUnknown*)*ppv);
//    }
//
//    return Original::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
//}

//HRESULT WINAPI Hook::CoGetClassObject(REFCLSID rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo, REFIID riid, LPVOID *ppv)
//{
//	if (riid == IID_IClassFactory)
//	{
//		ATL::CComPtr<IClassFactory> originalFactory;
//		HRESULT hr = Original::CoGetClassObject(rclsid, dwClsContext, pServerInfo, riid, (void**)&originalFactory);
//		if (FAILED(hr))
//			return hr;
//
//		return CSampleObjectProxyFactory::CreateFactory(originalFactory, ppv);
//	}
//
//	return Original::CoGetClassObject(rclsid, dwClsContext, pServerInfo, riid, ppv);
//}

HRESULT WINAPI Hook::DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID** ppv)
{
	if(rclsid == CLSID_AudioClient)
	{
		HRESULT hr = Original::DllGetClassObject(rclsid, riid, ppv);
		if (FAILED(hr))
			return hr;
		return InstallComInterfaceHooks((IUnknown*)*ppv, riid);//此处获得应该是类厂接口，进一步hook实例化接口的函数
	}
	return Original::DllGetClassObject(rclsid, riid, ppv);
}

//////////////////////////////////////////////////////////////////////////

void InstallHooks()
{
	DWORD dwError = NO_ERROR;
	HINSTANCE hInst;
	hInst=LoadLibrary(TEXT("Audioses.dll"));
	dwError = GetLastError();
    for(size_t i = 0; i < g_FunctionsCount; ++i)
    {
        if(!GetModuleHandleA(g_Functions[i].FunctionModule))
            throw std::runtime_error("Cannot find the module ");

        *g_Functions[i].OriginalFunction = GetProcAddress(GetModuleHandleA(g_Functions[i].FunctionModule), g_Functions[i].FunctionName);

        if(*g_Functions[i].OriginalFunction == NULL)
            throw std::runtime_error("Cannot find the function ");

        if(!Mhook_SetHook(g_Functions[i].OriginalFunction, g_Functions[i].HookFunction))
            throw std::runtime_error("Cannot set hook on the function ");
    }
}

void UninstallHooks()
{
    BOOL failed = false;

    for(size_t i = 0; i < g_FunctionsCount; ++i)
    {
        if (g_Functions[i].OriginalFunction)
            failed = failed || !Mhook_Unhook(g_Functions[i].OriginalFunction);
    }

    if (failed)
        throw std::runtime_error("UninstallHooks was failed to remove one or more hooks");
}