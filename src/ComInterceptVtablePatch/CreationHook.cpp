#include "StdAfx.h"
#include "CreationHook.h"

#include "Factory.h"
#include "VtableHooks.h"

//////////////////////////////////////////////////////////////////////////

typedef HRESULT (WINAPI *CoCreateInstance_T)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*);
typedef HRESULT (WINAPI *CoGetClassObject_T)(REFCLSID rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo, REFIID riid, LPVOID *ppv);

namespace Original
{
    CoCreateInstance_T  CoCreateInstance    = NULL;
    CoGetClassObject_T  CoGetClassObject    = NULL;
}

namespace Hook
{
    HRESULT WINAPI CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv);
    HRESULT WINAPI CoGetClassObject(REFCLSID rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo, REFIID riid, LPVOID *ppv);
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
    {"ole32.dll", "CoCreateInstance", (void**)&Original::CoCreateInstance, (void*)Hook::CoCreateInstance},
    {"ole32.dll", "CoGetClassObject", (void**)&Original::CoGetClassObject, (void*)Hook::CoGetClassObject}
};

const size_t g_FunctionsCount = sizeof(g_Functions)/sizeof(FunctionInfo);

//////////////////////////////////////////////////////////////////////////

HRESULT WINAPI Hook::CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv)
{
    if (rclsid == CLSID_SampleObject)
    {        
        if (pUnkOuter)
            return CLASS_E_NOAGGREGATION;

        HRESULT hr = Original::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
        if (FAILED(hr))
            return hr;

        return InstallComInterfaceHooks((IUnknown*)*ppv);
    }

    return Original::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

HRESULT WINAPI Hook::CoGetClassObject(REFCLSID rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo, REFIID riid, LPVOID *ppv)
{
    if (riid == IID_IClassFactory)
    {
        ATL::CComPtr<IClassFactory> originalFactory;
        HRESULT hr = Original::CoGetClassObject(rclsid, dwClsContext, pServerInfo, riid, (void**)&originalFactory);
        if (FAILED(hr))
            return hr;

        return CSampleObjectProxyFactory::CreateFactory(originalFactory, ppv);
    }

    return Original::CoGetClassObject(rclsid, dwClsContext, pServerInfo, riid, ppv);
}

//////////////////////////////////////////////////////////////////////////

void InstallHooks()
{
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