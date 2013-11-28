#include "stdafx.h"
#include "VtableHooks.h"

namespace Hook
{
    STDMETHODIMP get_ObjectName(IUnknown* This, BSTR* pVal);
    STDMETHODIMP DoWork(IUnknown* This, LONG arg1, LONG arg2, LONG* result);
    STDMETHODIMP QueryInterface(IUnknown* This, REFIID riid, void **ppvObject);
}

struct Context
{
    Context()
        : m_Name("Hooked object")
    {
    }

    PVOID m_OriginalQueryInterface;
    PVOID g_OriginalRelease;
    PVOID m_OriginalGetObjectName;
    PVOID m_OriginalDoWork;
    ATL::CComBSTR m_Name;
};

//////////////////////////////////////////////////////////////////////////

std::auto_ptr<Context> g_Context;

//////////////////////////////////////////////////////////////////////////

HRESULT HookMethod(IUnknown* original, PVOID proxyMethod, PVOID* originalMethod, DWORD vtableOffset)
{
    PVOID* originalVtable = *(PVOID**)original;

    if (originalVtable[vtableOffset] == proxyMethod)
        return S_OK;

    *originalMethod = originalVtable[vtableOffset];
    originalVtable[vtableOffset] = proxyMethod;

    return S_OK;
}

HRESULT InstallComInterfaceHooks(IUnknown* originalInterface)
{
    // Only single instance of a target object is supported in the sample
    if (g_Context.get())
        return E_FAIL;

    ATL::CComPtr<ISampleObject> so;
    HRESULT hr = originalInterface->QueryInterface(IID_ISampleObject, (void**)&so);
    if (FAILED(hr))
        return hr; // we need this interface to be present

    // remove protection from the vtable
    DWORD dwOld = 0;
    if(!::VirtualProtect(*(PVOID**)(originalInterface), sizeof(LONG_PTR), PAGE_EXECUTE_READWRITE, &dwOld))
        return E_FAIL;

    // hook interface methods
    g_Context.reset(new Context);
    HookMethod(so, (PVOID)Hook::QueryInterface, &g_Context->m_OriginalQueryInterface, 0);
    HookMethod(so, (PVOID)Hook::get_ObjectName, &g_Context->m_OriginalGetObjectName, 7);
    HookMethod(so, (PVOID)Hook::DoWork, &g_Context->m_OriginalDoWork, 9);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

typedef HRESULT (WINAPI *QueryInterface_T)(IUnknown* This, REFIID riid, void **ppvObject);

STDMETHODIMP Hook::QueryInterface(IUnknown* This, REFIID riid, void **ppvObject)
{
    QueryInterface_T qi = (QueryInterface_T)g_Context->m_OriginalQueryInterface;
    HRESULT hr = qi(This, riid, ppvObject);
    return hr;
}

STDMETHODIMP Hook::get_ObjectName(IUnknown* This, BSTR* pVal)
{
    return g_Context->m_Name.CopyTo(pVal);
}

STDMETHODIMP Hook::DoWork(IUnknown* This, LONG arg1, LONG arg2, LONG* result)
{
    *result = 42;
    return S_OK;
}