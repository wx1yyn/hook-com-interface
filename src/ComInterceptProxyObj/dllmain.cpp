#include "stdafx.h"
#include "CreationHook.h"

#include "ComSample_i.h"
#include "ComSample_i.c"

class CComInterceptModule : public ATL::CAtlDllModuleT<CComInterceptModule>
{
};

CComInterceptModule _AtlModule;

//////////////////////////////////////////////////////////////////////////

BOOL DllInit(HMODULE module)
{
    InstallHooks();
    return TRUE;
}

BOOL DllFree()
{
    UninstallHooks();
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    try
    {
        switch (reason)
        {
        case DLL_PROCESS_ATTACH:
            return DllInit(module);

        case DLL_PROCESS_DETACH:
            return DllFree();
        }

        return TRUE;
    }
    catch (const std::exception& )
    {
        return FALSE;
    }
}

